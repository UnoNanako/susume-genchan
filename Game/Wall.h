#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"

struct AABB;

class Wall : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	void SetModel(Model* model) { mModel = model;  }
	void SetAABBScale(Vector3 scale) { mAABBScale = scale; }
private:
	AABB mAABB;
	Vector3 mAABBScale;
};

