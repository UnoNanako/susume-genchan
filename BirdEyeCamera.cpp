#include "BirdEyeCamera.h"
#include "WinApiManager.h"
#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "MyMath.h"

BirdEyeCamera::BirdEyeCamera()
	:mTransform({ {1.0f,1.0f,1.0f},{0.5f,0.0f,0.0f},{0.0f,10.0f,-20.0f} })
	,mMatrix(MakeIdentity4x4())
	,mViewMatrix(MakeIdentity4x4())
	,mProjectionMatrix(MakeIdentity4x4())
{
}

void BirdEyeCamera::Initialize(DirectXCommon* dxCommon)
{
	mTransform = { {1.0f,1.0f,1.0f},{0.5f,0.0f,0.0f},{0.0f,10.0f,-20.0f} };
	cameraResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(Vector3));
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
}

void BirdEyeCamera::Update()
{
	ImGui::Begin("BirdCamera");
	ImGui::DragFloat3("Position", &mTransform.translate.x, 0.05f);
	ImGui::DragFloat3("Rotate", &mTransform.rotate.x, 0.05f);
	ImGui::End();
	mMatrix = MakeAffineMatrix(mTransform.scale, mTransform.rotate, mTransform.translate);
	mViewMatrix = Inverse(mMatrix);
	mProjectionMatrix = MakePerspectiveFovMatrix(50.0f * (kPi / 180.0f), WinApiManager::kClientWidth / float(WinApiManager::kClientHeight), 0.1f, 1000.0f);
	*cameraData = { mTransform.translate };
}

void BirdEyeCamera::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());
}
