#pragma once
#include "Transform.h"
#include <d3d12.h>
#include <memory>

class DirectXCommon;
class Texture;
class Input;

class TitleScene
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update(Input* input);
	void Draw(ID3D12GraphicsCommandList* commandList);
private:
	DirectXCommon* mDxCommon;
	std::unique_ptr<Texture> mTexture;
	Transform mTransform;
};

