#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"
#include <chrono>

struct AABB;
class Player;

class RotateEnemy : public GameObject
{
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
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }

private:
	uint32_t mRotateTimer;
	bool mIsRotate;
	AABB mAABB;
	float mFovAngle; //視野角
	float mLength;
	bool mIsPlayerInView; //プレイヤーが視野内にいるか
	std::chrono::steady_clock::time_point mLastRotationTime; //最後の回転時間
};

