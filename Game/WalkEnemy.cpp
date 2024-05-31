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
	, mVelocity({ 0.05f,0.0f,0.05f }){
}

WalkEnemy::~WalkEnemy(){
	delete mTexture;
	delete mModel;
}

void WalkEnemy::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f}, //scale
		{0.0f,0.0f,0.0f}, //rotate
		{0.0f,0.0f,0.0f} //translate
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
		mTransform.translate.z += mVelocity.z;
		if (mTransform.translate.z > 25.0f) {
			mDirection = RIGHT;
		}
		break;
	case DOWN:
		mTransform.translate.z -= mVelocity.z;
		if (mTransform.translate.z < 15.0f) {
			mDirection = LEFT;
		}
		break;
	case LEFT:
		mTransform.translate.x -= mVelocity.x;
		if (mTransform.translate.x < -17.5f) {
			mDirection = UP;
		}
		break;
	case RIGHT:
		mTransform.translate.x += mVelocity.x;
		if (mTransform.translate.x > -7.5f) {
			mDirection = DOWN;
		}
		break;
	}
	mTransform.UpdateMatrix();
	mAABB = CalcurateAABB(mTransform.translate);
}

void WalkEnemy::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB WalkEnemy::CalcurateAABB(const Vector3& translate){
	AABB ret;
	ret.min = {
		{translate.x - (5.0f / 2.0f)},
		{translate.y - (5.0f / 2.0f)},
		{translate.z - (5.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (5.0f / 2.0f)},
		{translate.y + (5.0f / 2.0f)},
		{translate.z + (5.0f / 2.0f)}
	};
	return ret;
}
