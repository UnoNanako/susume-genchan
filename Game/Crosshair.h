#pragma once
#include "Transform.h"
#include <d3d12.h>

class Sprite;
class DirectXCommon;

class Crosshair
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);
private:
	Sprite* mCrossSprite;
};

