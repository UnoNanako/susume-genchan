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
	,mIsAlive(true)
	, mLength(20.0f)
	, mVelocity({ 0.05f,0.0f,0.05f })
	,mGravity(0.05f){
}

WalkEnemy::~WalkEnemy(){
	delete mTexture;
	delete mModel;
}

void WalkEnemy::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f} 
	};
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/Model/Enemies/WalkEnemy/Atlas.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Enemies/WalkEnemy", "Goblin.obj");
	mModel->SetTexture(mTexture);
}

void WalkEnemy::Update(){
	switch (mDirection) {
	case UP:
		mTransform.rotate = { 0.0f,0.0f,0.0f };
		mTransform.translate.z += mVelocity.z;
		if (mTransform.translate.z > mMoveMax.z) {
			mDirection = RIGHT;
		}
		break;
	case DOWN:
		mTransform.rotate = { 0.0f,kPi,0.0f };
		mTransform.translate.z -= mVelocity.z;
		if (mTransform.translate.z < mMoveMin.z) {
			mDirection = LEFT;
		}
		break;
	case LEFT:
		mTransform.rotate = { 0.0f,-kPi / 2.0f,0.0f };
		mTransform.translate.x -= mVelocity.x;
		if (mTransform.translate.x < mMoveMin.x) {
			mDirection = UP;
		}
		break;
	case RIGHT:
		mTransform.rotate = { 0.0f,kPi/2.0f,0.0f };
		mTransform.translate.x += mVelocity.x;
		if (mTransform.translate.x > mMoveMax.x) {
			mDirection = DOWN;
		}
		break;
	}
	mTransform.UpdateMatrix();
	mAABB = CalculateAABB(mTransform.translate);
}

void WalkEnemy::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mTexture->Bind(commandList);
	if (mIsAlive == true) {
		mModel->Draw(commandList, camera, mTransform);
	}
}

AABB WalkEnemy::CalculateAABB(const Vector3& translate){
	AABB ret;
	ret.min = {
		{translate.x - (2.0f / 2.0f)},
		{translate.y - (2.0f / 2.0f)},
		{translate.z - (2.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (2.0f / 2.0f)},
		{translate.y + (2.0f / 2.0f)},
		{translate.z + (2.0f / 2.0f)}
	};
	return ret;
}
