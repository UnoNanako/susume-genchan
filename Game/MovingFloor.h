#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include <chrono>

class AABB;

class MovingFloor : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
private:
	AABB mAABB;
	float mSpeed;
	std::chrono::steady_clock::time_point mRoundtripTime; //往復する時間
};

