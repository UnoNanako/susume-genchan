#include "UpFloor.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void UpFloor::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mSpeed = 0.05f;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{7.5f,-4.0f,-17.5f,}
	};
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Floor/Up", "updownFloor.obj");
	mIsMove = false;
}

void UpFloor::Update()
{
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void UpFloor::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}

void UpFloor::Move()
{
	mTransform.translate.y += mSpeed;
	if (mTransform.translate.y >= 7.5f) {
		mSpeed = 0.0f;
		mIsMove = false;
	}
	mTransform.UpdateMatrix();
}

AABB UpFloor::CalcurateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{translate.x - (5.0f / 2.0f)},
		{translate.y - (10.0f / 2.0f)},
		{translate.z - (10.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (5.0f / 2.0f)},
		{translate.y + (10.0f / 2.0f)},
		{translate.z + (10.0f / 2.0f)}
	};
	return ret;
}
