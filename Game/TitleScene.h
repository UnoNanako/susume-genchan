#pragma once
#include "Transform.h"
#include <d3d12.h>

class DirectXCommon;
class Texture;
class Input;

class TitleScene
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update(Input* input);
	void Draw(DirectXCommon* dxCommon);
private:
	Texture* mTexture;
};

