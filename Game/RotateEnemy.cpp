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
	, mFovAngle(kPi / 2.0f) //90°
	,mIsPlayerInView(false)
	,mLength(20.0f)
{
}

void RotateEnemy::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform.translate = { 0.0f,0.0f,0.0f };
	mTransform.scale = { 1.0f,1.0f,1.0f };
	mTransform.rotate = { 0.0f,0.0f,0.0f };
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/enemies/rotateEnemy/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/enemies/rotateEnemy", "Giant.obj");
	mModel->SetTexture(mTexture);
}

void RotateEnemy::Update()
{
	ImGui::Begin("Debug");
	ImGui::DragFloat3("enemy translate", &mTransform.translate.x, 0.01f);
	ImGui::End();

	mAABB = CalcurateAABB(mTransform.translate);

	//現在の時間を取得
	auto now = std::chrono::steady_clock::now();
	if (mIsPlayerInView == false) {
		//5秒ごとに90°ずつ回転する
		if (std::chrono::duration_cast<std::chrono::seconds>(now - mLastRotationTime).count() >= 3) {
			mTransform.rotate.y += (kPi / 2);
			mLastRotationTime = now;
		}
	}
}

void RotateEnemy::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

//プレイヤーが視野角内に収まっているか判定する関数
bool RotateEnemy::DetectPlayer(Player* player)
{
	//プレイヤーの位置を取得
	Vector3 position = mTransform.translate;
	Vector3 playerPosition = player->GetTranslate();
	//敵からプレイヤーへのベクトルを計算する(正規化も)
	Vector3 toPlayer = playerPosition - position;
	toPlayer.Normalize();
	toPlayer.y = 0.0f;
	//敵の全歩を表すベクトルを計算する
	Vector3 forwardDirection = Multiply(Vector3(0.0f, 0.0f, 1.0f),MakeRotateYMatrix(mTransform.rotate.y)); //仮にZ軸が正面を向いていると仮定
	//プレイヤーが敵の視野角内に収まっているかどうかを判定する
	float dotProduct = Dot(forwardDirection, toPlayer); //内積を計算
	float fovCosine = cosf(mFovAngle * 0.5f);
	//内積が視野角の余弦以上であれば、プレイヤーは視野角内に収まっている
	if (dotProduct >= fovCosine) {
		return true;
	}
	else {
		return false;
	}
}

void RotateEnemy::TrackPlayer(Player* player)
{
	//プレイヤーが視野角内にいるかどうか
	if (DetectPlayer(player)) {
		//プレイヤーの位置を取得
		Vector3 playerPosition = player->GetTranslate();
		//敵からプレイヤーへのベクトルを計算する
		Vector3 toPlayer = playerPosition - mTransform.translate;
		if (Length(toPlayer) <= mLength) {
			mIsPlayerInView = true;
			toPlayer.Normalize();
			//敵の向きをプレイヤーの方向に向ける
			mTransform.rotate.y = toPlayer.y;
			mTransform.rotate.x = 0.0f;
			mTransform.rotate.y = atan2(toPlayer.x, toPlayer.z);
			mTransform.rotate.z = 0.0f;

			//敵をプレイヤーの方向に移動させる
			float moveSpeed = 0.1f;
			Vector3 moveDirection = toPlayer * moveSpeed;
			mTransform.translate += moveDirection;
		}
	}
	else {
		mIsPlayerInView = false;
	}
}

AABB RotateEnemy::CalcurateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{ translate.x - (5.0f / 2.0f) },
		{ translate.y - (5.0f / 2.0f) },
		{ translate.z - (5.0f / 2.0f) }
	};
	ret.max = {
		{ translate.x + (5.0f / 2.0f) },
		{ translate.y + (5.0f / 2.0f) },
		{ translate.z + (5.0f / 2.0f) }
	};
	return ret;
}
