#include "Ghost.h"
#include "DirectXCommon.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "Game/Player.h"

Ghost::Ghost()
	:mFovAngle(kPi / 2.0f) //90°
	, mIsPlayerInView(false)
	, mLength(20.0f)
	, mVelocity({ 0.05f,0.0f,0.0f })
	, mDirection(LEFT)
{
}

Ghost::~Ghost()
{
	delete mModel;
}

void Ghost::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = {
		{2.0f,2.0f,2.0f}, //scale
		{0.0f,0.0f,0.0f}, //rotate
		{0.0f,0.0f,0.0f} //transfrom
	};
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Enemies/Ghost", "ghost.obj");
}

void Ghost::Update()
{
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Ghost::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}

//プレイヤーが視野内に収まっているか判定する関数
bool Ghost::DetectPlayer(Player* player)
{
	//プレイヤーの位置を取得
	Vector3 position = mTransform.translate;
	Vector3 playerPosition = player->GetWorldPosition();
	//敵からプレイヤーへのベクトルを計算する(正規化も)
	Vector3 toPlayer = playerPosition - position;
	toPlayer.y = 0.0f;
	toPlayer.Normalize();
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
	}
	else {
		return false;
	}
}

void Ghost::TrackPlayer(Player* player)
{
	//プレイヤーの位置を取得
	Vector3 playerPosition = player->GetWorldPosition();
	//敵からプレイヤーへのベクトルを計算する
	Vector3 toPlayer = playerPosition - mTransform.translate;
	toPlayer.y = 0.0f;
	toPlayer.Normalize();
	//ゴーストの前方を表すベクトルを計算
	Vector3 forwardDirection = Multiply(Vector3(0.0f, 0.0f, 1.0f), MakeRotateYMatrix(mTransform.rotate.y));
	//プレイヤーの前方を表すベクトルを計算
	Vector3 playerForwardDirection = Multiply(Vector3(0.0f, 0.0f, 1.0f), MakeRotateYMatrix(player->GetTransform().rotate.y));
	forwardDirection.Normalize();
	playerForwardDirection.Normalize();
	//プレイヤーとゴーストが向き合っているかどうかを判定するため、内積を計算
	float dotProduct = Dot(forwardDirection, playerForwardDirection);
	//プレイヤーとゴーストが向き合っている場合
	if (dotProduct <= 0.0f) {
		//ゴーストを停止させる
		mIsPlayerInView = false;
	}
	else {
		//プレイヤーとゴーストが向き合っていない場合、プレイヤーを追尾する
		mIsPlayerInView = true;
	}

	//敵の向きをプレイヤーの方向に向ける
	//方向ベクトルを正規化し、敵の向きをプレイヤーの方向に向ける。
	//Y軸方向の回転角度を計算し、Y軸回りの回転を適用する。
	mTransform.rotate.x = 0.0f;
	mTransform.rotate.y = atan2(toPlayer.x, toPlayer.z);
	mTransform.rotate.z = 0.0f;

	//敵をプレイヤーの方向に移動させる
	float moveSpeed = 0.1f;
	Vector3 moveDirection = toPlayer * moveSpeed;
	if (mIsPlayerInView == true)
	{
		mTransform.translate += moveDirection;
	}
}

AABB Ghost::CalcurateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{translate.x - (mTransform.scale.x / 2.0f)},
		{translate.y - (mTransform.scale.y / 2.0f)},
		{translate.z - (mTransform.scale.z / 2.0f)}
	};
	ret.max = {
		{translate.x + (mTransform.scale.x / 2.0f)},
		{translate.y + (mTransform.scale.y / 2.0f)},
		{translate.z + (mTransform.scale.z / 2.0f)}
	};
	return ret;
}
