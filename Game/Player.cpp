#include "Player.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "PlayerCamera.h"
#include "VertexData.h"
#include "Input/Input.h"
#include "Light/LightList.h"
#include "externals/imgui/imgui.h"
#include <format>

Player::Player()
	:mIsHit(false)
	, mIsEnemyHit(false)
	, mIsAttack(false)
	, mIsOperatable(false)
	, mAABBtranslate({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} })
	, mLightList(nullptr)
	, mRotateSpeed(0.05f)
	, mSpeed(0.2f)
	, mTexture(nullptr)
	, mVelocity({ 0.1f,0.0f,0.1f })
	, mGravity(0.05f)
	, mHp(2)
	, mInvincibleTime(120)
	, mProgressTimer(5)
	, mBackTimer(5) {
}

Player::~Player() {
	delete mTexture;
	delete mModel;
}

void Player::Initialize(DirectXCommon* dxCommon) {
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f}, //scale
		{0.0f,0.0f,0.0f}, //rotate
		{-25.0f,5.0f,-25.0f} //translate
	};
	mAABBtranslate = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/Model/Player/Chick/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Player/Chick", "Chick.obj");
	mModel->SetTexture(mTexture);
}

void Player::Update(Input* input, float theta) {
	if (mIsHit == true) { //地面に当たったら
		//mTransform.translate.y = 3.0f;
		mVelocity.y = 0.0f;
	}
	mVelocity.y -= mGravity; //毎フレーム重力をかけている
	mTransform.translate.y += mVelocity.y;
	ImGui::Begin("Debug");
	ImGui::DragFloat3("pos", &mTransform.translate.x);
	ImGui::Text(std::format("{}", mVelocity.y).c_str());
	ImGui::End();
	//読む！理解する！
	//ここから
	//3軸の回転行列を作成
	Matrix4x4 rotationX = MakeRotateXMatrix(mTransform.rotate.x);
	Matrix4x4 rotationY = MakeRotateYMatrix(mTransform.rotate.y);
	Matrix4x4 rotationZ = MakeRotateZMatrix(mTransform.rotate.z);
	//3軸の回転行列を1つの行列に結合
	Matrix4x4 rotationMatrix = Multiply(rotationX, Multiply(rotationY, rotationZ));

	//ビューの座標系を変換するための行列
	//y軸回りの回転を行い、thetaとpi/2を引いた角度で回転
	mTransposeViewMatrix = MakeRotateYMatrix(-theta - kPi / 2.0f);
	//前方
	Vector3 frontVec;
	frontVec = { 0.0f,0.0f,mSpeed };
	//右方向
	Vector3 rightVec;
	rightVec = { mSpeed,0.0f,0.0f };
	//2つのベクトルをそれぞれビュー行列で変換。ビューに対して前方と右方向のベクトルが求まる
	frontVec = Multiply(frontVec, mTransposeViewMatrix);
	rightVec = Multiply(rightVec, mTransposeViewMatrix);
	//ここまで

	//キーボード
	if (mIsOperatable == true) {
		if (input->PushKey(DIK_W)) {
			mTransform.translate.x += frontVec.x;
			mTransform.translate.y += frontVec.y;
			mTransform.translate.z += frontVec.z;
		}
		if (input->PushKey(DIK_S)) {
			mTransform.translate.x -= frontVec.x;
			mTransform.translate.y -= frontVec.y;
			mTransform.translate.z -= frontVec.z;
		}
		if (input->PushKey(DIK_A)) {
			mTransform.translate.x -= rightVec.x;
			mTransform.translate.y -= rightVec.y;
			mTransform.translate.z -= rightVec.z;
		}
		if (input->PushKey(DIK_D)) {
			mTransform.translate.x += rightVec.x;
			mTransform.translate.y += rightVec.y;
			mTransform.translate.z += rightVec.z;
		}

		//ゲームパッド
		//Lスティック
		Vector2 lStick = input->GetLStick();
		frontVec.x *= lStick.y;
		frontVec.y *= lStick.y;
		frontVec.z *= lStick.y;
		rightVec.x *= lStick.x;
		rightVec.y *= lStick.x;
		rightVec.z *= lStick.x;
		mTransform.translate.x += frontVec.x;
		mTransform.translate.y += frontVec.y;
		mTransform.translate.z += frontVec.z;
		mTransform.translate.x += rightVec.x;
		mTransform.translate.y += rightVec.y;
		mTransform.translate.z += rightVec.z;

		//Rスティック
		Vector2 rStick = input->GetRStick();
		mTransform.rotate.y = -theta - kPi / 2.0f;

		//Bボタン
		if (input->GetButtonDown(XINPUT_GAMEPAD_B)) {
			mIsAttack = true;
		}
		if (mIsAttack == true && mProgressTimer > 0) {
			--mProgressTimer;
			Vector3 frontVec;
			frontVec = { 0.0f,0.0f,1.0f };
			frontVec = Multiply(frontVec, MakeRotateYMatrix(mTransform.rotate.y));
			mTransform.translate += frontVec;
		} else if (mIsAttack == true && mProgressTimer <= 0) {
			Vector3 frontVec;
			frontVec = { 0.0f,0.0f,1.0f };
			frontVec = Multiply(frontVec, MakeRotateYMatrix(mTransform.rotate.y));
			mTransform.translate -= frontVec;
			--mBackTimer;
			if (mBackTimer <= 0) {
				mIsAttack = false;
				mProgressTimer = 5;
				mBackTimer = 5;
			}
		}
	}

	mTransform.UpdateMatrix();
	Vector3 worldPos = GetWorldPosition();
	mAABBtranslate = CalculateAABB(worldPos);
	//リスポーン
	if (mTransform.translate.y <= -50.0f) {
		Initialize(mDxCommon);
	}

	//敵と当たっているとき
	if (mIsEnemyHit) {
		--mInvincibleTime;
		if (mInvincibleTime <= 0) {
			mIsEnemyHit = false;
			mInvincibleTime = 120;
		}
	}

	ImGui::Begin("Debug");
	ImGui::DragFloat3("player Position", &mTransform.translate.x, 0.01f);
	ImGui::DragFloat3("player Rotation", &mTransform.rotate.x, 0.01f);
	ImGui::End();
}

void Player::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) {
	mLightList->SetSpotLightPos(mTransform.translate);
	Matrix4x4 rotationX = MakeRotateXMatrix(mTransform.rotate.x);
	Matrix4x4 rotationY = MakeRotateYMatrix(mTransform.rotate.y);
	Matrix4x4 rotationZ = MakeRotateZMatrix(mTransform.rotate.z);
	Matrix4x4 rotationMatrix = Multiply(rotationX, Multiply(rotationY, rotationZ));
	Vector3 frontVec;
	frontVec = { 0.0f,0.0f,1.0f };
	frontVec = Multiply(frontVec, rotationMatrix);
	mLightList->SetSpotLightDirection(frontVec);

	mTexture->Bind(commandList);
	if ((mInvincibleTime / 5) % 2 == 0) {
		mModel->Draw(commandList, camera, mTransform);
	}
}

AABB Player::CalculateAABB(const Vector3& translate) {
	AABB ret;
	ret.min = {
		{translate.x - (2.0f / 2.0f)},
		{translate.y - (2.0f / 2.0f)},
		{translate.z - (2.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (2.0f / 2.0f)},
		{translate.y + (2.0f / 2.0f)},
		{translate.z + (2.0f / 2.0f)}
	};
	return ret;
}
