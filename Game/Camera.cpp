#include "Camera.h"
#include "WinApiManager.h"
#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "MyMath.h"

void Camera::Initialize(DirectXCommon* dxCommon)
{
	mTransform = { {1.0f,1.0f,1.0f},{0.8f,0.0f,0.0f},{0.0f,95.0f,-100.0f} };
	cameraResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(),sizeof(Vector3));
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
}

void Camera::Update()
{
	ImGui::Begin("Debug");
	ImGui::DragFloat3("Camera Position", &mTransform.translate.x, 0.05f);
	ImGui::DragFloat3("Camera Rotate", &mTransform.rotate.x, 0.05f);
	ImGui::End();
	const float kPi = std::numbers::pi_v<float>;
	mMatrix = MakeAffineMatrix(mTransform.scale,mTransform.rotate,mTransform.translate);
	mViewMatrix = Inverse(mMatrix);
	mProjectionMatrix = MakePerspectiveFovMatrix(50.0f * (kPi / 180.0f), WinApiManager::kClientWidth / float(WinApiManager::kClientHeight), 0.1f, 1000.0f);
	*cameraData = { mTransform.translate };
}

void Camera::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());
}
