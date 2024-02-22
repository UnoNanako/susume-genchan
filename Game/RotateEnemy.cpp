#include "RotateEnemy.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "externals/imgui/imgui.h"

void RotateEnemy::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform.translate = { 0.0f,0.0f,0.0f };
	mTransform.scale = { 10.0f,10.0f,10.0f };
	mTransform.rotate = { 0.0f,0.0f,0.0f };
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/enemies/rotateEnemy/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/enemies/rotateEnemy", "Giant.obj");
	mModel->SetTexture(mTexture);
}

void RotateEnemy::Update()
{
	ImGui::Begin("Debug");
	ImGui::DragFloat3("enemy translate", &mTransform.translate.x, 0.01f);
	ImGui::End();
}

void RotateEnemy::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}
