#include "PlayerCamera.h"
#include "WinApiManager.h"
#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "MyMath.h"

PlayerCamera::PlayerCamera(){
	mTransform = { {1.0f,1.0f,1.0f},{0.8f,0.0f,0.0f},{0.0f,95.0f,-100.0f} };
}

void PlayerCamera::Update(){
#ifdef DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", &mTransform.translate.x, 0.05f);
	ImGui::DragFloat3("Rotate", &mTransform.rotate.x, 0.05f);
	ImGui::End();
#endif // DEBUG
	
	const float kPi = std::numbers::pi_v<float>;
	mMatrix = MakeAffineMatrix(mTransform.scale,mTransform.rotate,mTransform.translate);
	mViewMatrix = Inverse(mMatrix);
	mProjectionMatrix = MakePerspectiveFovMatrix(50.0f * (kPi / 180.0f), WinApiManager::kClientWidth / float(WinApiManager::kClientHeight), 0.1f, 1000.0f);
	*cameraData = { mTransform.translate };
}
