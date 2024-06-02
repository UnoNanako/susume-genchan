#include "MiniBridge.h"
#include "VertexData.h"
#include "Engine/3D/Model.h"
#include "Engine/DirectXCommon.h"

MiniBridge::MiniBridge()
{
}

MiniBridge::~MiniBridge()
{
	delete mModel;
}

void MiniBridge::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {5.0f,5.0f,5.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/MiniBridge", "MiniBridge.obj");
}

void MiniBridge::Update()
{
	mTransform.UpdateMatrix();
	mAABB = CalculateAABB(mTransform.translate);
}

void MiniBridge::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}

AABB MiniBridge::CalculateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{translate.x - (5.0f / 2.0f)},
		{translate.y - (5.0f / 2.0f) - 2.0f},
		{translate.z - (5.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (5.0f / 2.0f)},
		{translate.y + (5.0f / 2.0f) - 2.0f},
		{translate.z + (5.0f / 2.0f)}
	};
	return ret;
}
