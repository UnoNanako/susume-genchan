#include "MovingFloor.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void MovingFloor::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {10.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "floor.obj");
}

void MovingFloor::Update()
{
}

void MovingFloor::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}
