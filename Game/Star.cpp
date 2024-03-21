#include "Star.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void Star::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { 
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/star", "star.obj");
}

void Star::Update()
{
	mTransform.rotate.y += 0.05f;
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Star::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}

AABB Star::CalcurateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{translate.x - (2.0f / 2.0f)},
		{translate.y - (2.0f / 2.0f)},
		{translate.z - (2.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (2.0f / 2.0f)},
		{translate.y + (2.0f / 2.0f)},
		{translate.z + (2.0f / 2.0f)}
	};
	return ret;
}