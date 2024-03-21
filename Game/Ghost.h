#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"

class Player;
class AABB;

class Ghost : public GameObject
{
public:
	Ghost();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
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
	AABB mAABB;
	float mFovAngle; //視野角
	float mLength;
	bool mIsPlayerInView; //プレイヤーが視野内にいるか
	Vector3 mVelocity;

	enum Direction {
		LEFT,
		RIGHT
	};
	Direction mDirection; //向き
};

