#include "BirdEyeCamera.h"
#include "WinApiManager.h"
#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "MyMath.h"
#include "Engine/Input/Input.h"

BirdEyeCamera::BirdEyeCamera(){
	mTransform = { {1.0f,1.0f,1.0f},{0.8f,0.0f,0.0f},{-21.0f,8.0f,-19.0f} };
	mTarget = { 0.0f,0.0f,0.0f };
	mUp = { 0.0f,1.0f,0.0f };
	mViewMatrix = CreateLookAt(mTransform.translate, mTarget, mUp);
	mRadius = 50.0f;
	mLat = 1.0f;
	mLon = -kPi/2.0f;
	mT = 0.0f;
	mStartPos = mTransform.translate;
}

void BirdEyeCamera::Update(Input* input, Vector3 playerTranslate,bool isTitleScene){
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
	
	//線形補間
	if (!isTitleScene) {
		mT += 0.05f;
		if (mT > 1.0f) {
			mT = 1.0f;
		}
		float x = mRadius * sin(mLat) * cos(mLon) + playerTranslate.x;
		float y = mRadius * cos(mLat) + playerTranslate.y;
		float z = mRadius * sin(mLat) * sin(mLon) + playerTranslate.z;
		Vector3 tmp = { x - mStartPos.x,y - mStartPos.y,z - mStartPos.z };
		tmp.x = mStartPos.x + (x - mStartPos.x) * mT;
		tmp.y = mStartPos.y + (y - mStartPos.y) * mT;
		tmp.z = mStartPos.z + (z - mStartPos.z) * mT;
		mTransform.translate.x = tmp.x;
		mTransform.translate.y = tmp.y;
		mTransform.translate.z = tmp.z;
	}
	mViewMatrix = CreateLookAt(mTransform.translate, playerTranslate, mUp);
	mProjectionMatrix = MakePerspectiveFovMatrix(50.0f * (kPi / 180.0f), WinApiManager::kClientWidth / float(WinApiManager::kClientHeight), 0.1f, 1000.0f);
	*cameraData = { mTransform.translate };
}

