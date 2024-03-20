#include "WalkEnemy.h"
#include "DirectXCommon.h"
#include "PlayerCamera.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "Game/Player.h"
#include "externals/imgui/imgui.h"

WalkEnemy::WalkEnemy()
	:mFovAngle(kPi / 2.0f) //90°
	, mIsPlayerInView(false)
	, mLength(20.0f)
	, mVelocity({ 0.05f,0.0f,0.05f })
{
}

void WalkEnemy::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f}, //scale
		{0.0f,0.0f,0.0f}, //rotate
		{0.0f,0.0f,0.0f} //translate
	};
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/enemies/walkEnemy/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/enemies/walkEnemy", "Goblin.obj");
	mModel->SetTexture(mTexture);
}

void WalkEnemy::Update()
{
	switch (mDirection) {
	case UP:
		mTransform.translate.z += mVelocity.z;
		if (mTransform.translate.z >= 25.0f) {
			mDirection = RIGHT;
		}
		break;
	case DOWN:
		mTransform.translate.z -= mVelocity.z;
		if (mTransform.translate.z <= 15.0f) {
			mDirection = LEFT;
		}
		break;
	case LEFT:
		mTransform.translate.x -= mVelocity.x;
		if (mTransform.translate.x <= 17.5f) {
			mDirection = UP;
		}
		break;
	case RIGHT:
		mTransform.translate.x += mVelocity.x;
		if (mTransform.translate.x >= 25.0f) {
			mDirection = DOWN;
		}
		break;
	}
	mTransform.UpdateMatrix();
}

void WalkEnemy::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}
