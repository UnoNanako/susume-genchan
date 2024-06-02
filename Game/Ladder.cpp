#include "Ladder.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

Ladder::~Ladder(){
	delete mTexture;
}

void Ladder::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/Model/Ladder/color.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Ladder", "ladder.obj");
	mModel->SetTexture(mTexture);
}

void Ladder::Update(){
	mAABB = CalculateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Ladder::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Ladder::CalculateAABB(const Vector3& translate){
	AABB ret;
	ret.min = {
		{translate.x - (1.0f / 2.0f)},
		{translate.y - (mHeight / 2.0f)},
		{translate.z - (1.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (1.0f / 2.0f)},
		{translate.y + (mHeight / 2.0f)},
		{translate.z + (1.0f / 2.0f)}
	};
	return ret;
}
