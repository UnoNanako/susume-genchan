#pragma once
#include "GameObject.h"
#include "Game/MoveFloor.h"
#include "Transform.h"
#include <d3d12.h>
#include <chrono>

class AABB;

class SlideFloor : public MoveFloor
{
public:
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	Transform& GetTransform() { return mTransform; }
	Vector3 GetWorldPosition() { return Vector3{ mTransform.mMatWorld.m[3][0],mTransform.mMatWorld.m[3][1],mTransform.mMatWorld.m[3][2] }; }
	Matrix4x4 GetWorldMatrix() { return mTransform.mMatWorld; }
	AABB GetAABB() { return mAABB; }
	bool GetIsMoving() { return mIsMoving; }
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	void SetIsMoving(bool isMoving) { mIsMoving = isMoving; }

private:
	AABB mAABB;
	float mSpeed;
	std::chrono::steady_clock::time_point mRoundtripTime; //往復する時間
	bool mIsMoving; //動いているか
};

