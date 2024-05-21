#include "Gem.h"
#include "VertexData.h"
#include "Engine/2D/Texture.h"
#include "Engine/DirectXCommon.h"
#include "Engine/3D/Model.h"
#include "Easing.h"

Gem::~Gem(){
	delete mTexture;
	delete mModel;
}

void Gem::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = { {1.0f,1.0f,1.0f} ,{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f} };
	mTexture = new Texture;
	mGetColor = std::make_unique<Texture>();
	mGetColor->Create(mDxCommon, "resources/Model/Gem/gemGet.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Gem", "Gem_gltf.gltf");
	mModel->SetAnimation(mModel->LoadAnimationFile("resources/Model/Gem", "Gem_gltf.gltf"));
}

void Gem::Update(){
	mTransform.rotate.y += 0.05f;
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
	if (mIsHit) {
		mModel->Update();
	}
}

void Gem::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mGetColor->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Gem::CalcurateAABB(const Vector3& translate){
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
