#include "Camera.h"
#include "DirectXCommon.h"
#include "WinApiManager.h"

void Camera::Initialize(DirectXCommon* dxCommon)
{
	cameraResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(Vector3));
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
}

void Camera::Update()
{
}

void Camera::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());
}
