#include "Player.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "Camera.h"
#include "VertexData.h"
#include "Input/Input.h"
#include "Light/LightList.h"
#include "externals/imgui/imgui.h"

void Player::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mVelocity = { 0.1f,0.0f,0.1f };
	mTransform.translate = { -25.0f,-5.0f,-25.0f };
	mTransform.rotate = { 0.0f,0.0f,0.0f };
	mTransform.scale = { 1.0f,1.0f,1.0f };
	mAABBtranslate = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mIsHit = false;
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/uvChecker.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "floor.obj");
	mModel->SetTexture(mTexture);
	mSpeed = 0.25f;
	mRotateSpeed = 0.05f;
}

void Player::Update(Input* input)
{
	//読む！理解する！
	//ここから
	Matrix4x4 rotationX = MakeRotateXMatrix(mTransform.rotate.x);
	Matrix4x4 rotationY = MakeRotateYMatrix(mTransform.rotate.y);
	Matrix4x4 rotationZ = MakeRotateZMatrix(mTransform.rotate.z);
	Matrix4x4 rotationMatrix = Multiply(rotationX, Multiply(rotationY, rotationZ));
	Vector3 frontVec;
	frontVec = {0.0f,0.0f,mSpeed};
	Vector3 rightVec;
	rightVec = { mSpeed,0.0f,0.0f };
	frontVec = Multiply(frontVec, rotationMatrix);
	rightVec = Multiply(rightVec, rotationMatrix);
	//ここまで

	//キーボード
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
	if (input->PushKey(DIK_LEFT)) {
		mTransform.rotate.y -= 0.05f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		mTransform.rotate.y += 0.05f;
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
	mTransform.rotate.y += (rStick.x * mRotateSpeed);

	if (mIsHit == true) {
		mVelocity = { 0.0f,0.0f,0.0f };
	}
	mAABBtranslate = {
		{mTransform.translate.x - mTransform.scale.x,mTransform.translate.y - mTransform.scale.y,mTransform.translate.z - mTransform.scale.z},
		{mTransform.translate.x + mTransform.scale.x,mTransform.translate.y + mTransform.scale.y,mTransform.translate.z + mTransform.scale.z}
	};
	ImGui::Begin("Debug");
	ImGui::DragFloat3("player Position", &mTransform.translate.x, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("player Rotation", &mTransform.rotate.x, 0.01f, 0.0f, 10.0f);
	ImGui::Checkbox("isPlayerCamera2", &mIsPlayerCamera);
	ImGui::End();
}

void Player::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mLightList->SetSpotLightPos(mTransform.translate);
	Matrix4x4 rotationX = MakeRotateXMatrix(mTransform.rotate.x);
	Matrix4x4 rotationY = MakeRotateYMatrix(mTransform.rotate.y);
	Matrix4x4 rotationZ = MakeRotateZMatrix(mTransform.rotate.z);
	Matrix4x4 rotationMatrix = Multiply(rotationX, Multiply(rotationY, rotationZ));
	Vector3 frontVec;
	frontVec = { 0.0f,0.0f,1.0f };
	frontVec = Multiply(frontVec, rotationMatrix);
	mLightList->SetSpotLightDirection(frontVec);
	if (mIsPlayerCamera == true) {
		camera->SetTransform(mTransform);
	}
	else {
		camera->SetTransform({ {1.0f,1.0f,1.0f},{0.8f,0.0f,0.0f},{0.0f,95.0f,-100.0f} });
	}
	//mTexture->Bind(commandList);
	//mModel->Draw(commandList, camera,mTransform);
}