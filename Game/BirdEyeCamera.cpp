#include "BirdEyeCamera.h"
#include "WinApiManager.h"
#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "MyMath.h"
#include "Engine/Input/Input.h"

BirdEyeCamera::BirdEyeCamera(){
	mTransform = { {1.0f,1.0f,1.0f},{0.8f,0.0f,0.0f},{-50.0f,35.0f,-55.0f} };
	mTarget = { 0.0f,0.0f,0.0f };
	mUp = { 0.0f,1.0f,0.0f };
	mViewMatrix = CreateLookAt(mTransform.translate, mTarget, mUp);
	mRadius = 50.0f;
	mLat = 1.0f;
	mLon = 0.0f;
}

void BirdEyeCamera::Update(Input* input, Vector3 playerTranslate){
	ImGui::Begin("BirdCamera");
	ImGui::DragFloat3("Position", &mTransform.translate.x, 0.05f);
	ImGui::DragFloat3("Rotate", &mTransform.rotate.x, 0.05f);
	ImGui::End();

	if (mIsHit == false) {
		//ゲームパッドの右スティックの入力を取得
		Vector2 rSttick = input->GetRStick();
		//カメラの方位角と仰角を更新
		mLon -= rSttick.x * 0.02f;
		mLat += rSttick.y * 0.02f;

		mLat = Max((kPi / 18.0f), Min(mLat, static_cast<float>(kPi / 2)));
		if (input->PushKey(DIK_LEFT)) {
			mLon += 0.01f;
		}
		if (input->PushKey(DIK_RIGHT)) {
			mLon -= 0.01f;
		}
		if (input->PushKey(DIK_UP)) {
			mLat += 0.01f;
		}
		if (input->PushKey(DIK_DOWN)) {
			mLat -= 0.01f;
		}
	}
	
	float x = mRadius * sin(mLat) * cos(mLon) + playerTranslate.x;
	float y = mRadius * cos(mLat) + playerTranslate.y;
	float z = mRadius * sin(mLat) * sin(mLon) +playerTranslate.z;
	mTransform.translate.x = x;
	mTransform.translate.y = y;
	mTransform.translate.z = z;

	mViewMatrix = CreateLookAt(mTransform.translate, playerTranslate, mUp);
	mProjectionMatrix = MakePerspectiveFovMatrix(50.0f * (kPi / 180.0f), WinApiManager::kClientWidth / float(WinApiManager::kClientHeight), 0.1f, 1000.0f);
	*cameraData = { mTransform.translate };
}

