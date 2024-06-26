#include "RotateEnemy.h"
#include "DirectXCommon.h"
#include "PlayerCamera.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "Game/Player.h"
#include "externals/imgui/imgui.h"

RotateEnemy::RotateEnemy()
	:mRotateTimer(180)
	, mIsRotate(false)
	, mIsAlive(true)
	, mFovAngle(kPi / 2.0f) //90°
	, mIsPlayerInView(false)
	, mLength(20.0f)
	, mState(EnemyState::ROTATE) {
}

RotateEnemy::~RotateEnemy() {
	delete mTexture;
	delete mModel;
}

void RotateEnemy::Initialize(DirectXCommon* dxCommon) {
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f}, //scale
		{0.0f,0.0f,0.0f}, //rotate
		{0.0f,0.0f,0.0f} //transform
	};
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/Model/Enemies/RotateEnemy/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Enemies/RotateEnemy", "Giant.obj");
	mModel->SetTexture(mTexture);
	mInitTranslate = { 15.0f,2.0f,12.5f }; //初期位置を保存
}

void RotateEnemy::Update(Player* player) {
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::DragFloat3("enemy translate", &mTransform.translate.x, 0.01f);
	ImGui::End();
#endif // DEBUG

	switch (mState) {
	case EnemyState::ROTATE:
		Rotate(player);
		break;
	case EnemyState::CHASE:
		Chase(player);
		break;
	case EnemyState::RETURN:
		Return(player);
		break;
	}

	mAABB = CalculateAABB(mTransform.translate);

	mTransform.UpdateMatrix();
}

void RotateEnemy::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) {
	mTexture->Bind(commandList);
	if (mIsAlive == true) {
		mModel->Draw(commandList, camera, mTransform);
	}
}

//プレイヤーが視野角内に収まっているか判定する関数
bool RotateEnemy::DetectPlayer(Player* player) {
	//プレイヤーの位置を取得
	Vector3 position = mTransform.translate;
	Vector3 playerPosition = player->GetTranslate();
	//敵からプレイヤーへのベクトルを計算する(正規化も)
	Vector3 toPlayer = playerPosition - position;
	toPlayer.Normalize();
	toPlayer.y = 0.0f;
	//敵の前方を表すベクトルを計算する
	//敵の向きはY軸回りに回転していると考え、Z軸が正面を向いていると仮定して回転行列を使って計算
	Vector3 forwardDirection = Multiply(Vector3(0.0f, 0.0f, 1.0f), MakeRotateYMatrix(mTransform.rotate.y)); //仮にZ軸が正面を向いていると仮定
	//プレイヤーが敵の視野角内に収まっているかどうかを判定する
	//敵の前方ベクトルと敵からプレイヤーへの方向ベクトルの間の内積を計算し、その値が視野角の余弦以上かどうかで行う。
	float dotProduct = Dot(forwardDirection, toPlayer); //内積を計算
	float fovCosine = cosf(mFovAngle * 0.5f);
	//内積が視野角の余弦以上であれば、プレイヤーは視野角内に収まっている
	if (dotProduct >= fovCosine) {
		return true;
	} else {
		return false;
	}
}

void RotateEnemy::TrackPlayer(Player* player) {
	//プレイヤーが視野角内にいるかどうか
	if (DetectPlayer(player)) {
		//プレイヤーの位置を取得
		Vector3 playerPosition = player->GetTranslate();
		//敵からプレイヤーへのベクトルを計算する
		Vector3 toPlayer = playerPosition - mTransform.translate;
		//↑のベクトルの長さがmLengthよりも短い場合、プレイヤーが敵の攻撃範囲内にいると判断される。
		if (Length(toPlayer) <= mLength) {
			mState = EnemyState::CHASE;
			toPlayer.Normalize();
			//敵の向きをプレイヤーの方向に向ける
			//方向ベクトルを正規化し、敵の向きをプレイヤーの方向に向ける。
			//Y軸方向の回転角度を計算し、Y軸回りの回転を適用する。
			mTransform.rotate.y = toPlayer.y;
			mTransform.rotate.x = 0.0f;
			mTransform.rotate.y = atan2(toPlayer.x, toPlayer.z);
			mTransform.rotate.z = 0.0f;

			//敵をプレイヤーの方向に移動させる
			mMoveSpeed = 0.08f;
			Vector3 moveDirection = toPlayer * mMoveSpeed;
			mTransform.translate.x += moveDirection.x;
			mTransform.translate.z += moveDirection.z;
		} else {
			mState = EnemyState::RETURN;
		}
	} else {
		mState = EnemyState::RETURN;
	}
}

void RotateEnemy::Rotate(Player* player) {
	//現在の時間を取得
	auto now = std::chrono::steady_clock::now();
	//5秒ごとに90°ずつ回転する
	if (std::chrono::duration_cast<std::chrono::seconds>(now - mLastRotationTime).count() >= 3) {
		mTransform.rotate.y += (kPi / 2);
		mLastRotationTime = now;
	}
	if (DetectPlayer(player)) {
		//プレイヤーの位置を取得
		Vector3 playerPosition = player->GetTranslate();
		//敵からプレイヤーへのベクトルを計算する
		Vector3 toPlayer = playerPosition - mTransform.translate;
		//↑のベクトルの長さがmLengthよりも短い場合、プレイヤーが敵の攻撃範囲内にいると判断される。
		if (Length(toPlayer) <= mLength) {
			mState = EnemyState::CHASE;
		}
	}
}

void RotateEnemy::Chase(Player* player) {
	//プレイヤーが視野角内にいるかどうか
	if (DetectPlayer(player)) {
		//プレイヤーの位置を取得
		Vector3 playerPosition = player->GetTranslate();
		//敵からプレイヤーへのベクトルを計算する
		Vector3 toPlayer = playerPosition - mTransform.translate;
		//↑のベクトルの長さがmLengthよりも短い場合、プレイヤーが敵の攻撃範囲内にいると判断される。
		if (Length(toPlayer) <= mLength) {
			toPlayer.Normalize();
			//敵の向きをプレイヤーの方向に向ける
			//方向ベクトルを正規化し、敵の向きをプレイヤーの方向に向ける。
			//Y軸方向の回転角度を計算し、Y軸回りの回転を適用する。
			mTransform.rotate.y = toPlayer.y;
			mTransform.rotate.x = 0.0f;
			mTransform.rotate.y = atan2(toPlayer.x, toPlayer.z);
			mTransform.rotate.z = 0.0f;

			//敵をプレイヤーの方向に移動させる
			mMoveSpeed = 0.1f;
			Vector3 moveDirection = toPlayer * mMoveSpeed;
			mTransform.translate.x += moveDirection.x;
			mTransform.translate.z += moveDirection.z;
		} else {
			mState = EnemyState::RETURN;
		}
	} else {
		mState = EnemyState::RETURN;
	}
}

void RotateEnemy::Return(Player* player) {
	//プレイヤーが視野内にいなければ初期位置に戻る
	Vector3 vec = mInitTranslate - mTransform.translate;
	if (Length(vec) <= 0.15f) {
		mTransform.rotate = { 0.0f,0.0f,0.0f };
		mTransform.rotate = { 0.0f,0.0f,0.0f };
		mState = EnemyState::ROTATE;
		return;
	}
	//初期位置の方へ向く
	mTransform.rotate.x = 0.0f;
	mTransform.rotate.y = atan2(vec.x, vec.z);
	mTransform.rotate.z = 0.0f;
	vec.Normalize();
	//初期位置まで動く
	vec *= mMoveSpeed;
	mTransform.translate += vec;
	if (DetectPlayer(player)) {
		//プレイヤーの位置を取得
		Vector3 playerPosition = player->GetTranslate();
		//敵からプレイヤーへのベクトルを計算する
		Vector3 toPlayer = playerPosition - mTransform.translate;
		//↑のベクトルの長さがmLengthよりも短い場合、プレイヤーが敵の攻撃範囲内にいると判断される。
		if (Length(toPlayer) <= mLength) {
			mState = EnemyState::CHASE;
		}
	}
}

AABB RotateEnemy::CalculateAABB(const Vector3& translate) {
	AABB ret;
	ret.min = {
		{ translate.x - (4.0f / 2.0f) },
		{ translate.y - (4.0f / 2.0f) },
		{ translate.z - (4.0f / 2.0f) }
	};
	ret.max = {
		{ translate.x + (4.0f / 2.0f) },
		{ translate.y + (4.0f / 2.0f) },
		{ translate.z + (4.0f / 2.0f) }
	};
	return ret;
}
