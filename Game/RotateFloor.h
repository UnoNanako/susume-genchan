#pragma once
#include "Engine/GameObject.h"
#include "Game/MoveFloor.h"
#include "Transform.h"
#include <d3d12.h>

class AABB;

class RotateFloor : public MoveFloor
{
public:
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
private:
	AABB mAABB;
};

