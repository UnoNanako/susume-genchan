#include "TitleScene.h"
#include "Engine/DirectXCommon.h"
#include "Engine/2D/Sprite.h"

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTitleSprite = std::make_unique<Sprite>();
	mTitleSprite->Create(mDxCommon, "resources/Sprite/Text/title.png");
	mTitleSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,100.0f,0.0f} });
	mAbuttonSprite = std::make_unique<Sprite>();
	mAbuttonSprite->Create(mDxCommon, "resources/Sprite/Ui/Buttons/xbox_button_color_a.png");
	mAbuttonSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{620.0f,550.0f,0.0f} });
}

void TitleScene::Update()
{
	mTitleSprite->Update();
	mAbuttonSprite->Update();
}

void TitleScene::Draw(ID3D12GraphicsCommandList* commandList)
{
	mTitleSprite->Draw(commandList);
	mAbuttonSprite->Draw(commandList);
}
