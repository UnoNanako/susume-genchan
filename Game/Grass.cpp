#include "Grass.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

Grass::~Grass(){
	delete mTexture;
	delete mModel;
}

void Grass::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mTexture = new Texture();
	mTexture->Create(dxCommon, "resources/Model/Blocks/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Blocks/Bush", "Bush.obj");
	mModel->SetTexture(mTexture);
}

void Grass::Update(){
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Grass::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Grass::CalcurateAABB(const Vector3& translate){
	AABB ret;
	ret.min = {
		{translate.x - (3.0f / 2.0f)},
		{translate.y - (3.0f / 2.0f)},
		{translate.z - (3.0f / 2.0f)}
	};					
	ret.max = {			
		{translate.x + (3.0f / 2.0f)},
		{translate.y + (3.0f / 2.0f)},
		{translate.z + (3.0f / 2.0f)}
	};
	return ret;
}
