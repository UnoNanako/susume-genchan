#include "RotateFloor.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void RotateFloor::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{37.5f,0.0f,-15.0f}
	};
	mTexture = new Texture();
	mTexture->Create(dxCommon, "resources/Model/Floor/Rotate/red.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Floor/Rotate", "rotate.obj");
	mModel->SetTexture(mTexture);
}

void RotateFloor::Update()
{
	mTransform.UpdateMatrix();
}

void RotateFloor::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}
