#include "Fence.h"
#include "VertexData.h"
#include "Engine/3D/Model.h"
#include "Engine/DirectXCommon.h"

Fence::Fence()
{
}

Fence::~Fence()
{
	delete mModel;
}

void Fence::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {5.0f,5.0f,5.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Fence", "Fence.obj");
}

void Fence::Update()
{
	mTransform.UpdateMatrix();
	mAABB = CalculateAABB(mTransform.translate);
}

void Fence::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}

AABB Fence::CalculateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{translate.x - (5.0f / 2.0f)},
		{translate.y - (5.0f / 2.0f)},
		{translate.z - (5.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (5.0f / 2.0f)},
		{translate.y + (5.0f / 2.0f)},
		{translate.z + (5.0f / 2.0f)}
	};
	return ret;
}
