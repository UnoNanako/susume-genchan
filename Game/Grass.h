#pragma once
#include "GameObject.h"
#include <d3d12.h>

class AABB;

class Grass : public GameObject{
public:
	~Grass();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
private:
	AABB mAABB;
};

