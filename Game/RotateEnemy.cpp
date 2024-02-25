#include "RotateEnemy.h"
#include "DirectXCommon.h"
#include "PlayerCamera.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "externals/imgui/imgui.h"

RotateEnemy::RotateEnemy()
	:mRotateTimer(180)
	,mIsRotate(false)
{
}

void RotateEnemy::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform.translate = { 0.0f,0.0f,0.0f };
	mTransform.scale = { 1.0f,1.0f,1.0f };
	mTransform.rotate = { 0.0f,0.0f,0.0f };
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/enemies/rotateEnemy/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/enemies/rotateEnemy", "Giant.obj");
	mModel->SetTexture(mTexture);
}

void RotateEnemy::Update()
{
	ImGui::Begin("Debug");
	ImGui::DragFloat3("enemy translate", &mTransform.translate.x, 0.01f);
	ImGui::End();

	mAABB = CalcurateAABB(mTransform.translate);

	/*--mRotateTimer;
	if (mRotateTimer <= 0) {
		mIsRotate = true;
	}
	if (mIsRotate == true) {

	}*/
}

void RotateEnemy::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB RotateEnemy::CalcurateAABB(const Vector3& translate)
{
	AABB ret;
	ret.min = {
		{ translate.x - (5.0f / 2.0f) },
		{ translate.y - (5.0f / 2.0f) },
		{ translate.z - (5.0f / 2.0f) }
	};
	ret.max = {
		{ translate.x + (5.0f / 2.0f) },
		{ translate.y + (5.0f / 2.0f) },
		{ translate.z + (5.0f / 2.0f) }
	};
	return ret;
}
