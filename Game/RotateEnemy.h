#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"
#include <vector>

class DirectXCommon;
class Camera;
class Model;
struct AABB;
class Player;

class RotateEnemy : public GameObject
{
public:
	RotateEnemy();
	void Initialize(DirectXCommon* dxCommon)  override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) override;
	//プレイヤーが視野角内に収まっているか判定する関数
	bool DetectPlayer(Player* player);
	void TrackPlayer(Player* player);

	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);
	AABB GetAABB() { return mAABB; }
	Vector3 GetTranslate() { return mTransform.translate; }
	void SetTranslate(const Vector3& translate) { mTransform.translate = translate; }
private:
	uint32_t mRotateTimer;
	bool mIsRotate;
	AABB mAABB;
	float mFovAngle; //視野角
};

