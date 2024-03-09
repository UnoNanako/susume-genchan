#pragma once
#include "GameObject.h"
#include <d3d12.h>

class AABB;

class Switch : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon)override;
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
private:
	AABB mAABB;
	Texture* mRed;
	Texture* mBlue;

	bool mIsPushed; //スイッチが押されているかフラグ
};

