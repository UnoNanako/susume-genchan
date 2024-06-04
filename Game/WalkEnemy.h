#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"

class Player;
struct AABB;

class WalkEnemy : public GameObject {
public:
	WalkEnemy();
	~WalkEnemy();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);
	//プレイヤーが視野角内に収まっているか判定する関数
	bool DetectPlayer(Player* player);
	bool TrackPlayer(Player* player);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	bool GetIsAlive() { return mIsAlive; }
	void SetIsAlive(bool isAlive) { mIsAlive = isAlive; }
	void SetMoveMax(Vector3 moveMax) { mMoveMax = moveMax; }
	void SetMoveMin(Vector3 moveMin) { mMoveMin = moveMin; }

private:
	float mFovAngle; //視野角
	float mLength; //視線の長さ
	bool mIsPlayerInView; //プレイヤーが視野内にいるか
	bool mIsAlive; //生きているか
	float mGravity; //重力
	Vector3 mVelocity;
	AABB mAABB; //当たり判定
	Vector3 mMoveMax; //移動範囲の最大
	Vector3 mMoveMin; //移動範囲の最小

	enum Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
	Direction mDirection = DOWN;
};

