#include "TitleScene.h"
#include "Engine/DirectXCommon.h"
#include "Engine/2D/Texture.h"

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{100.0f,100.0f,0.0f} };
	mTexture = std::make_unique<Texture>();
	mTexture->Create(mDxCommon, "resources/Sprite/Text/title.png");
}

void TitleScene::Update(Input* input)
{
}

void TitleScene::Draw(ID3D12GraphicsCommandList* commandList)
{
	mTexture->Bind(commandList);
}
