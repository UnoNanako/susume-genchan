#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class DirectXCommon;
class AABB;

class Seed : public GameObject
{
public:
	~Seed();
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minAmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	AABB GetAABB() { return mAABB; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }
	bool GetIsHit() { return mIsHit; }

private:
	AABB mAABB;
	bool mIsHit = false;
};

