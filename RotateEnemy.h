#pragma once
#include "GameObject.h"
#include <d3d12.h>

class DirectXCommon;
class Camera;

class RotateEnemy : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon)  override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) override;

private:
};

