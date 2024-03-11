#include "Switch.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "Game/MoveFloor.h"

void Switch::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = { {5.0f,5.0f,5.0f},{0.0f,0.0f,0.0f},{32.5f,0.0f,-20.0f} };

	mMoveFloor = new MoveFloor();

	mRed = new Texture();
	mBlue = new Texture();
	mRed->Create(mDxCommon, "resources/blocks/switch/red.png");
	mBlue->Create(mDxCommon, "resources/blocks/switch/blue.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "floor.obj");
	mModel->SetTexture(mRed);
}

void Switch::Update()
{
	mAABB = CalcurateAABB(mTransform.translate);
	mTransform.UpdateMatrix();
}

void Switch::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mRed->Bind(commandList);
	mBlue->Bind(commandList);
	mModel->Draw(commandList, camera, mTransform);
}

AABB Switch::CalcurateAABB(const Vector3& translate)
{
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
