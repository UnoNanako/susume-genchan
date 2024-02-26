#include "BirdEyeCamera.h"
#include "WinApiManager.h"
#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "MyMath.h"

BirdEyeCamera::BirdEyeCamera()
{
	mTransform = { {1.0f,1.0f,1.0f},{0.8f,0.0f,0.0f},{0.0f,50.0f,-45.0f} };
	mTarget = { 0.0f,0.0f,0.0f };
	mUp = { 0.0f,1.0f,0.0f };
	mViewMatrix = CreateLookAt(mTransform.translate, mTarget, mUp);
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

void BirdEyeCamera::UpdateCameraPosition(const Vector3& translate, float angle, float distance)
{
	//カメラの位置を計算
	mTransform.translate.x = translate.x + distance * cos(angle);
	mTransform.translate.z = translate.z + distance * sin(angle);
	//注視点はプレイヤーの位置を設定
	mTarget = translate;
}
