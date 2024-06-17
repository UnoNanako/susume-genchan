#include "RotateFloor.h"
#include "Transform.h"
#include "3D/Model.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "Math/MyMath.h"

RotateFloor::~RotateFloor() {
	delete mTexture;
	delete mModel;
}

void RotateFloor::Initialize(DirectXCommon* dxCommon) {
	mDxCommon = dxCommon;
	mTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,kPi / 2.0f,0.0f},
		{15.0f,1.5f,57.5f}
	};
	mModel = new Model();
	mModel->Create(mDxCommon, "resources/Model/Bridge", "Bridge.obj");
}

void RotateFloor::Update() {
	Vector3 sideLWorldPos = { 6.5f,0.0f,0.0f };
	Vector3 sideRWorldPos = { -6.5f,0.0f,0.0f };
	Matrix4x4 rotateX = MakeRotateXMatrix(mTransform.rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(mTransform.rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(mTransform.rotate.z);
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);
	sideLWorldPos = Multiply(sideLWorldPos, rotateMatrix);
	sideRWorldPos = Multiply(sideRWorldPos, rotateMatrix);
	Vector3 WorldPos = GetWorldPosition();
	mSideLOBB = CalcurateOBB(sideLWorldPos + WorldPos, { 1.0f,10.0f,30.0f });
	mSideROBB = CalcurateOBB(sideRWorldPos + WorldPos, { 1.0f,10.0f,30.0f });
	mOBB = CalcurateOBB(WorldPos, { 5.5f,0.1f,30.0f });
	mTransform.UpdateMatrix();
}

void RotateFloor::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) {
	mModel->Draw(commandList, camera, mTransform);
}

OBB RotateFloor::CalcurateOBB(const Vector3& translate, const Vector3& size) {
	OBB ret;
	ret.center = translate;
	ret.size = size;
	Matrix4x4 rotateX = MakeRotateXMatrix(mTransform.rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(mTransform.rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(mTransform.rotate.z);
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);
	ret.axis[0].x = rotateMatrix.m[0][0];
	ret.axis[0].y = rotateMatrix.m[0][1];
	ret.axis[0].z = rotateMatrix.m[0][2];
	ret.axis[1].x = rotateMatrix.m[1][0];
	ret.axis[1].y = rotateMatrix.m[1][1];
	ret.axis[1].z = rotateMatrix.m[1][2];
	ret.axis[2].x = rotateMatrix.m[2][0];
	ret.axis[2].y = rotateMatrix.m[2][1];
	ret.axis[2].z = rotateMatrix.m[2][2];
	return ret;
}
