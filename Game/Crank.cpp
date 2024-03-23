#include "Crank.h"
#include "Engine/Input/Input.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"

void Crank::Initialize(DirectXCommon* dxCommon)
{
	mCalculateAngle = 0.0f;
	mCurrentAngle = 0.0f;
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,7.5f,12.5f}
	};
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/uvChecker.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "floor.obj");
	mModel->SetTexture(mTexture);
}

void Crank::Update(Input* input)
{
	mAABB = CalculateAABB(mTransform.translate);
	//Lスティック
	Vector2 lStick = input->GetLStick();
	float length = Length(lStick);
	if (length > 0.0f && mIsHit == true) {
		lStick.Normalize();
		mCalculateAngle = mCurrentAngle; //前のフレームの角度
		mCurrentAngle = atan2(lStick.y, lStick.x); //ラジアンが求まる
		float angle = mCurrentAngle - mCalculateAngle;
		if (angle > 0.05f) {
			angle = 0.05f;
		}
		if (angle < -0.05f) {
			angle = -0.05f;
		}		
		mTransform.rotate.y -= angle;
	}
	mTransform.UpdateMatrix();
}

void Crank::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Crank::CalculateAABB(const Vector3& translate)
{
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
