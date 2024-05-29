#pragma once
#include "Transform.h"
#include <d3d12.h>
#include <memory>

class DirectXCommon;
class Sprite;
class Input;

class TitleScene
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);
private:
	DirectXCommon* mDxCommon;
	std::unique_ptr<Sprite> mTitleSprite;
	std::unique_ptr<Sprite> mAbuttonSprite;
};

