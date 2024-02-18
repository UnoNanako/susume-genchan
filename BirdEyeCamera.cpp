#include "BirdEyeCamera.h"
#include "WinApiManager.h"
#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "MyMath.h"

void BirdEyeCamera::Initialize(DirectXCommon* dxCommon)
{
	transform = { {1.0f,1.0f,1.0f},{0.5f,0.0f,0.0f},{0.0f,10.0f,-20.0f} };
	cameraResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(Vector3));
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
}

void BirdEyeCamera::Update()
{
	ImGui::Begin("BirdCamera");
	ImGui::DragFloat3("Position", &transform.translate.x, 0.05f);
	ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.05f);
	ImGui::End();
	matrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	viewMatrix = Inverse(matrix);
	projectionMatrix = MakePerspectiveFovMatrix(50.0f * (kPi / 180.0f), WinApiManager::kClientWidth / float(WinApiManager::kClientHeight), 0.1f, 1000.0f);
	*cameraData = { transform.translate };
}

void BirdEyeCamera::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());
}
