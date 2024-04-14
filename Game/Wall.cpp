#include "Wall.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void Wall::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mAABBScale = { 0.0f,0.0f,0.0f };
	mTexture = new Texture();
	mTexture->Create(dxCommon, "resources/Model/Blocks/Grass/Blocks_PixelArt.png");
	mModel = new Model();
	mModel->SetTexture(mTexture);
}

void Wall::Update()
{
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Wall::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Wall::CalcurateAABB(const Vector3& translate)
{
	AABB ret; 
	ret.min = {
		{translate.x - (mAABBScale.x / 2.0f)},
		{translate.y - (mAABBScale.y / 2.0f)},
		{translate.z - (mAABBScale.z / 2.0f)}
	};
	ret.max = {
		{translate.x + (mAABBScale.x / 2.0f)},
		{translate.y + (mAABBScale.y / 2.0f)},
		{translate.z + (mAABBScale.z / 2.0f)}
	};
	return ret;
}
