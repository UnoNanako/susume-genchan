#include "MovingFloor.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void MovingFloor::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mSpeed = 2.0f;
	mTransform = { {10.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{ 15.0f,0.0f,32.5f } };
	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "floor.obj");
}

void MovingFloor::Update()
{
	mTransform.translate.z += mSpeed;
	//現在の時間を取得
	auto now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(now - mRoundtripTime).count() >= 5) {
		mSpeed *= -1;
	}
}

void MovingFloor::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}
