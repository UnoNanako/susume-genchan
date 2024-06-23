#include "Seed.h"
#include "VertexData.h"
#include "Engine/2D/Texture.h"
#include "Engine/DirectXCommon.h"
#include "Engine/3D/Model.h"

Seed::~Seed(){
	delete mModel;
}

void Seed::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Seed", "Seed.obj");
}

void Seed::Update(){
	mTransform.rotate.y += 0.05f;
	mAABB = CalculateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Seed::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	if (mIsHit == false) {
		mModel->Draw(commandList, camera, mTransform);
	}
}

AABB Seed::CalculateAABB(const Vector3& translate){
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
