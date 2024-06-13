#include "Gem.h"
#include "VertexData.h"
#include "Engine/2D/Texture.h"
#include "Engine/DirectXCommon.h"
#include "Engine/3D/Model.h"
#include "Engine/Particle/ParticleList.h"

Gem::~Gem(){
	delete mTexture;
	delete mModel;
}

void Gem::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = { {1.0f,1.0f,1.0f} ,{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mTexture = new Texture;
	mGetColor = std::make_unique<Texture>();
	mGetColor->Create(mDxCommon, "resources/Model/Gem/gemGet.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Gem", "Gem_gltf.gltf");
	mModel->SetAnimation(mModel->LoadAnimationFile("resources/Model/Gem", "Gem_gltf.gltf"));
	mModel->Update();
	mParticle = std::make_unique<ParticleList>();
	mParticle->Create(mDxCommon);
	mParticle->SetTranslateMin({ -1.0f,-1.0f,-1.0f });
	mParticle->SetTranslateMax({ 1.0f,1.0f,1.0f });
	mParticle->SetVelocityMin({ -5.0f,5.0f,-5.0f });
	mParticle->SetVelocityMax({ 5.0f,6.0f,5.0f });
	mParticle->SetColorMin({ 0.0f,0.0f,0.0f });
	mParticle->SetColorMax({ 1.0f,1.0f,1.0f });
	mParticle->SetLifeTImeMin(0.5f);
	mParticle->SetLifeTimeMax(1.0f);
}

void Gem::Update(){
	mTransform.rotate.y += 0.05f;
	mAABB = CalculateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
	if (mIsHit) {
		mModel->Update();
	}
	mParticle->Update();
	mParticle->SetEmitTranslate({ mTransform.translate.x,mTransform.translate.y,mTransform.translate.z });
	mParticle->SetParticleScale({ 1.0f,1.0f,1.0f });
}

void Gem::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mGetColor->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

void Gem::ParticleDraw(ID3D12GraphicsCommandList* commandList, Camera* camera) {
	mParticle->Draw(commandList, camera);
}

AABB Gem::CalculateAABB(const Vector3& translate){
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
