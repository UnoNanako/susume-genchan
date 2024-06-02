#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <d3d12.h>

class DirectXCommon;
struct AABB;

class MiniBridge : public GameObject {
public:
	MiniBridge();
	~MiniBridge();
	void Initialize(DirectXCommon* dxCommon) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) override;
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }

private:
	AABB mAABB;
};

