#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class DirectXCommon;
struct AABB;

class Seed : public GameObject
{
public:
	~Seed();
	void Initialize(DirectXCommon* dxCommon) override;
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minAmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

private:
	AABB mAABB;
	bool mIsHit = false;
};

