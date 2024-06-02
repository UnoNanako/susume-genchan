#pragma once
#include "GameObject.h"
#include "Game/MoveFloor.h"
#include "Transform.h"
#include <d3d12.h>

class AABB;

class UpFloor : public MoveFloor{
public:
	~UpFloor();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	void Move();
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	Transform& GetTransform() { return mTransform; }
	Vector3 GetWorldPosition() { return Vector3{ mTransform.mMatWorld.m[3][0],mTransform.mMatWorld.m[3][1],mTransform.mMatWorld.m[3][2] }; }
	Matrix4x4 GetWorldMatrix() { return mTransform.mMatWorld; }
	bool GetIsMove() { return mIsMove; }
	void SetIsMove(bool isMove) { mIsMove = isMove; }
private:
	AABB mAABB;
	float mSpeed;
};

