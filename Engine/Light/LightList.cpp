#include "LightList.h"
#include "DirectXCommon.h"
#include "MyMath.h"
#include "Transform.h"
#include "VertexData.h"
#include "externals/imgui/imgui.h"

void LightList::Create(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	mDirectionalLightResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(DirectionalLightForGPU));
	mDirectionalLightData = nullptr;
	mDirectionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&mDirectionalLightData));
	mDirectionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	mDirectionalLightData->direction = { 0.0f,-1.0f,0.0f };
	mDirectionalLightData->intensity = 0.0f;

	mPointLightResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(PointLightForGPU));
	mPointLightData = nullptr;
	mPointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&mPointLightData));
	mPointLightData->color = { 0.0f,0.0f,0.0f,1.0f };
	mPointLightData->position = { 0.0f,0.0f,0.0f };
	mPointLightData->intensity = 0.0f;
	mPointLightData->radius = 10.0f;
	mPointLightData->decay = 1.0f;

	mSpotLightResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(SpotLightForGPU));
	mSpotLightData = nullptr;
	mSpotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&mSpotLightData));
	mSpotLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	mSpotLightData->position = { 2.0f,1.25f,0.0f };
	mSpotLightData->distance = 150.0f;
	mSpotLightData->direction = Normalize({ 1.0f,0.0f,0.0f });
	mSpotLightData->intensity = 4.0f;
	mSpotLightData->decay = 2.0f;
	mSpotLightData->cosAngle = 0.9f;
	mSpotLightData->cosFalloffStart = 1.8f;
}

void LightList::Update()
{
#ifdef DEBUG

	ImGui::Begin("Debug");
	ImGui::DragFloat3("Position", &mSpotLightData->position.x, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("Rotation", &mSpotLightData->direction.x, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Distance", &mSpotLightData->distance, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("Intensity", &mSpotLightData->intensity, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Decay", &mSpotLightData->decay, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("CosFalloffStart", &mSpotLightData->cosFalloffStart, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("CosAngle", &mSpotLightData->cosAngle, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("Color", &mSpotLightData->color.x, 0.01f, 0.0f, 10.0f);
	ImGui::End();
#endif // DEBUG

}

void LightList::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootConstantBufferView(3, mDirectionalLightResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(5, mPointLightResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(6, mSpotLightResource->GetGPUVirtualAddress());
}
