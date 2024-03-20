#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"

class AABB;
class Player;

enum Direction {
	UP, 
	DOWN,
	LEFT,
	RIGHT
};

class WalkEnemy : public GameObject
{
public:
	WalkEnemy();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	//プレイヤーが視野角内に収まっているか判定する関数
	bool DetectPlayer(Player* player);
	bool TrackPlayer(Player* player);

private:
	float mFovAngle; //視野角
	float mLength;
	bool mIsPlayerInView; //プレイヤーが視野内にいるか
	Vector3 mVelocity;
	Direction mDirection = DOWN;
};

