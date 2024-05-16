#pragma once
#include "GameObject.h"
#include <d3d12.h>

class AABB;

class Star : public GameObject{
public:
	~Star();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	AABB GetAABB() { return mAABB; }

private:
	AABB mAABB;
};

