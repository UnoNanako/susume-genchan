#include "Gem.h"
#include "VertexData.h"
#include "Engine/2D/Texture.h"
#include "Engine/DirectXCommon.h"
#include "Engine/3D/Model.h"

void Gem::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {1.0f,1.0f,1.0f} ,{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f} };
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/gem/gemColor.png");
	mGetColor = new Texture();
	mGetColor->Create(mDxCommon, "resources/gem/gemGet.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/gem", "Gem.obj");
	mModel->SetTexture(mTexture);
}

void Gem::Update()
{
	mTransform.rotate.y += 0.05f;
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Gem::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mGetColor->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Gem::CalcurateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{translate.x - (1.0f / 2.0f)},
		{translate.y - (1.0f / 2.0f)},
		{translate.z - (1.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (1.0f / 2.0f)},
		{translate.y + (1.0f / 2.0f)},
		{translate.z + (1.0f / 2.0f)}
	};
	return ret;
}
