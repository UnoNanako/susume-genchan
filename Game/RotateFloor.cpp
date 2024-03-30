#include "RotateFloor.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "Math/MyMath.h"

void RotateFloor::Initialize(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{36.0f,0.0f,-15.0f}
	};
	mTexture = new Texture();
	mTexture->Create(dxCommon, "resources/Model/Floor/Rotate/red.png");
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Floor/Rotate", "rotate.obj");
	mModel->SetTexture(mTexture);
}

void RotateFloor::Update()
{
	Vector3 WorldPos = GetWorldPosition();
	CalcurateOBB(WorldPos);
	mTransform.UpdateMatrix();
}

void RotateFloor::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	mModel->Draw(commandList, camera, mTransform);
}

void RotateFloor::CalcurateOBB(const Vector3& translate)
{
	mOBB.center = translate;
	mOBB.size = { 2.5f,2.5f,5.0f };
	Matrix4x4 rotateX = MakeRotateXMatrix(mTransform.rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(mTransform.rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(mTransform.rotate.z);
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);
	mOBB.axis[0].x = rotateMatrix.m[0][0];
	mOBB.axis[0].y = rotateMatrix.m[1][0];
	mOBB.axis[0].z = rotateMatrix.m[2][0];
	mOBB.axis[1].x = rotateMatrix.m[0][1];
	mOBB.axis[1].y = rotateMatrix.m[1][1];
	mOBB.axis[1].z = rotateMatrix.m[2][1];
	mOBB.axis[2].x = rotateMatrix.m[0][2];
	mOBB.axis[2].y = rotateMatrix.m[1][2];
	mOBB.axis[2].z = rotateMatrix.m[2][2];
}
