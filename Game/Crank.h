#pragma once
#include "Engine/GameObject.h"
#include "Transform.h"
#include <d3d12.h>
#include <vector>

class Input;
class AABB;

class Crank : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update(Input* input);
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	bool GetIsHit() { return mIsHit; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }
	
private:
	AABB mAABB;
	float mCalculateAngle;
	float mCurrentAngle;
	bool mIsHit = false;
};

