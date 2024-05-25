#include "TitleScene.h"
#include "Engine/DirectXCommon.h"
#include "Engine/2D/Sprite.h"

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mSprite = std::make_unique<Sprite>();
	mSprite->Create(mDxCommon, "resources/Sprite/Text/title.png");
	mSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
}

void TitleScene::Update()
{
	mSprite->Update();
}

void TitleScene::Draw(ID3D12GraphicsCommandList* commandList)
{
	mSprite->Draw(commandList);
}
