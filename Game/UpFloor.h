#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <d3d12.h>

class AABB;

class UpFloor : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);
private:
	AABB mAABB;
	float mSpeed;
	bool mIsMoving; //動いているか
};

