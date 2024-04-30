#include "Skydome.h"
#include "3D/Model.h"
#include "VertexData.h"

Skydome::~Skydome()
{
	delete mModel;
}

void Skydome::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Skydome", "sky.obj");
}

void Skydome::Update()
{
	mTransform.UpdateMatrix();
}

void Skydome::Draw(ID3D12GraphicsCommandList* commansList, Camera* camera)
{
	mModel->Draw(commansList, camera, mTransform);
}
