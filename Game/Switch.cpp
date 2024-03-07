#include "Switch.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void Switch::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {5.0f,5.0f,5.0f},{0.0f,0.0f,0.0f},{32.5f,0.0f,-20.0f} };
	mRed = new Texture();
	mBlue = new Texture();
	mRed->Create(mDxCommon, "resources/blocks/switch/red.png");
	mBlue->Create(mDxCommon, "resources/blocks/switch/blue.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "floor.obj");
	mModel->SetTexture(mRed);
}

void Switch::Update()
{
}

void Switch::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mRed->Bind(commandList);
	mBlue->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}
