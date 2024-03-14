#include "Gem.h"
#include "VertexData.h"
#include "Engine/2D/Texture.h"
#include "Engine/DirectXCommon.h"
#include "Engine/3D/Model.h"

void Gem::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {1.0f,1.0f,1.0f} ,{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f} };
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/gem/gemColor.png");
	mGetColor = new Texture();
	mGetColor->Create(mDxCommon, "resources/gem/gemGet.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/gem", "Gem.obj");
	mModel->SetTexture(mTexture);
}

void Gem::Update()
{
	mTransform.rotate.y += 0.05f;
	mTransform.UpdateMatrix();
}

void Gem::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mGetColor->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}
