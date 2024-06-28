#include "Star.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "externals/imgui/imgui.h"
#include "Engine/Particle/ParticleList.h"

Star::~Star() {
	delete mModel;
}

void Star::Initialize(DirectXCommon* dxCommon) {
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Star", "star.obj");
	mParticle = std::make_unique<ParticleList>();
	mParticle->Create(mDxCommon, "resources/Particle/circle.png");
	mParticle->SetTranslateMin({ -1.0f,-1.0f,-1.0f });
	mParticle->SetTranslateMax({ 1.0f,1.0f,1.0f });
	mParticle->SetVelocityMin({ -5.0f,5.0f,-5.0f });
	mParticle->SetVelocityMax({ 5.0f,6.0f,5.0f });
	mParticle->SetColorMin({ 150.0f/255.0f,150.0f/255.0f,0.0f });
	mParticle->SetColorMax({ 1.0f,1.0f,0.0f });
	mParticle->SetLifeTImeMin(0.5f);
	mParticle->SetLifeTimeMax(1.0f);
}
  
void Star::Update() {
	mTransform.rotate.y += 0.05f;
	mAABB = CalculateAABB(mTransform.translate);
	mParticle->Update();
	mParticle->SetEmitTranslate({ mTransform.translate.x,mTransform.translate.y,mTransform.translate.z });
	mParticle->SetParticleScale({ 1.0f,1.0f,1.0f });
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::DragFloat3("Star", &mTransform.translate.x, 0.01f);
	ImGui::End();
#endif // DEBUG
	mTransform.UpdateMatrix();
}

void Star::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) {
	mModel->Draw(commandList, camera, mTransform);
}

void Star::ParticleDraw(ID3D12GraphicsCommandList* commandList, Camera* camera) {
	mParticle->Draw(commandList, camera);
}

AABB Star::CalculateAABB(const Vector3& translate) {
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
