#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"
#include <vector>

class DirectXCommon;
class Camera;
class Model;
struct AABB;

class RotateEnemy : public GameObject
{
public:
	RotateEnemy();
	void Initialize(DirectXCommon* dxCommon)  override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) override;
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);
	AABB GetAABB() { return mAABB; }

	void SetTranslate(const Vector3& translate) { mTransform.translate = translate; }
private:
	uint32_t mRotateTimer;
	bool mIsRotate;
	AABB mAABB;
};

