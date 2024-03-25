#include "Crosshair.h"
#include "2D/Sprite.h"
#include "DirectXCommon.h"

void Crosshair::Initialize(DirectXCommon* dxCommon)
{
	mCrossSprite = new Sprite();
	mCrossSprite->Create(dxCommon, "resources/Sprite/Crosshair/Cross.png");
	mCrossSprite->SetTransform({ { 1.0f,1.0f,1.0f },{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
}

void Crosshair::Update()
{
	mCrossSprite->Update();
}

void Crosshair::Draw(ID3D12GraphicsCommandList* commandList)
{
	mCrossSprite->Draw(commandList);
}
