#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"
#include <chrono>

struct AABB;
class Player;

class RotateEnemy : public GameObject{
public:
	RotateEnemy();
	~RotateEnemy();
	void Initialize(DirectXCommon* dxCommon)  override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) override;
	//プレイヤーが視野角内に収まっているか判定する関数
	bool DetectPlayer(Player* player);
	void TrackPlayer(Player* player);
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	bool GetIsAlive() { return mIsAlive; }
	void SetIsAlive(bool isAlive) { mIsAlive = isAlive; }

private:
	uint32_t mRotateTimer;
	bool mIsRotate;
	AABB mAABB;
	float mMoveSpeed;
	float mFovAngle; //視野角
	float mLength;
	bool mIsPlayerInView; //プレイヤーが視野内にいるか
	bool mIsAlive; //生きているか
	std::chrono::steady_clock::time_point mLastRotationTime; //最後の回転時間
	Vector3 mInitTranslate; //初期位置
};

