#include "Switch.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "Game/MoveFloor.h"

Switch::~Switch(){
	delete mMoveFloor;
	delete mRed;
	delete mBlue;
	delete mModel;
}

void Switch::Initialize(DirectXCommon* dxCommon){
	mDxCommon = dxCommon;
	mTransform = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	mMoveFloor = new MoveFloor();

	mRed = new Texture();
	mBlue = new Texture();
	mRed->Create(mDxCommon, "resources/Model/Gimmick/Switch/red.png");
	mBlue->Create(mDxCommon, "resources/Model/Gimmick/Switch/blue.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Gimmick/Switch", "switch.obj");
	mModel->SetTexture(mRed);
}

void Switch::Update(){
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Switch::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera){
	mRed->Bind(commandList);
	mBlue->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Switch::CalcurateAABB(const Vector3& translate){
	AABB ret;
	ret.min = {
		{translate.x - (1.0f / 2.0f)},
		{translate.y - (1.0f / 2.0f)},
		{translate.z - (1.0f / 2.0f)}
	};
	ret.max = {
		{translate.x + (1.0f / 2.0f)},
		{translate.y + (1.0f / 2.0f)},
		{translate.z + (1.0f / 2.0f)}
	};
	return ret;
}
