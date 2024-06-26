#include "Crank.h"
#include "Engine/Input/Input.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "2D/Sprite.h"
#include "VertexData.h"

Crank::Crank()
	:mCalculateAngle(0.0f)
	,mCurrentAngle(0.0f){
}

Crank::~Crank(){
	delete mTexture;
	delete mModel;
	delete mFoundationModel;
}

void Crank::Initialize(DirectXCommon* dxCommon){
	mCalculateAngle = 0.0f;
	mCurrentAngle = 0.0f;
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,kPi/2.0f,0.0f},
		{0.0f,18.0f,12.5f}
	};
	mFoundationTransform = {
		{2.0f,2.0f,2.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,17.5f,12.5f}
	};
	mTexture = new Texture();
	mTexture->Create(mDxCommon, "resources/Model/Gimmick/Crank/red.png");
	//クランク本体
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Gimmick/Crank", "Crank.obj");
	mModel->SetTexture(mTexture);
	//土台
	mFoundationModel = new Model();
	mFoundationModel->Create(mDxCommon, "resources/Model/Gimmick/Foundation", "Foundation.obj");
	//LTボタン
	mRStickSprite = std::make_unique<Sprite>();
	mRStickSprite->Create(mDxCommon, "resources/Sprite/Ui/Buttons/xbox_stick_top_r.png");
	mRStickSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{600.0f,500.0f,0.0f} });
}

void Crank::Update(Input* input){
	mAABB = CalculateAABB(mTransform.translate);
	//Rスティック
	Vector2 rStick = input->GetRStick();
	float length = Length(rStick);
	if (length > 0.0f && mIsHit == true) {
		rStick.Normalize();
		mCalculateAngle = mCurrentAngle; //前のフレームの角度
		mCurrentAngle = atan2(rStick.y, rStick.x); //ラジアンが求まる
		float angle = mCurrentAngle - mCalculateAngle;
		if (angle > 0.05f) {
			angle = 0.02f;
		}
		if (angle < -0.05f) {
			angle = -0.02f;
		}		
		mTransform.rotate.y -= angle;
	}
	mTransform.UpdateMatrix();
	mFoundationTransform.UpdateMatrix();
	mRStickSprite->Update();
}

void Crank::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mTexture->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
	mFoundationModel->Draw(commandList, camera, mFoundationTransform);
	if (mIsHit) {
		mRStickSprite->Draw(commandList);
	}
}

AABB Crank::CalculateAABB(const Vector3& translate){
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
