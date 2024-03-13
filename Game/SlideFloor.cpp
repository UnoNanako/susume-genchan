#include "SlideFloor.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void SlideFloor::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mSpeed = -0.15f;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{15.0f,0.0f,32.5f}
	};
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/moveFloor", "moveFloor.obj");
	mIsMove = false;
}

void SlideFloor::Update()
{
	if (mIsMove == true) {
		mTransform.translate.z += mSpeed;
		//現在の時間を取得
		auto now = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(now - mRoundtripTime).count() >= 5) {
			mSpeed *= -1;
			mRoundtripTime = now;
		}
	}
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void SlideFloor::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}

AABB SlideFloor::CalcurateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{translate.x - (10.0f / 2.0f)},
		{translate.y - (5.0f / 2.0f)},
		{translate.z - (10.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (10.0f / 2.0f)},
		{translate.y + (5.0f / 2.0f)},
		{translate.z + (10.0f / 2.0f)}
	};
	return ret;
}
