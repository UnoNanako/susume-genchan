#pragma once
#include "Engine/GameObject.h"
#include "Game/MoveFloor.h"
#include "Transform.h"
#include <d3d12.h>

struct OBB;

class RotateFloor : public MoveFloor{
public:
	~RotateFloor();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;

	Vector3 GetWorldPosition() { return Vector3{ mTransform.mMatWorld.m[3][0],mTransform.mMatWorld.m[3][1],mTransform.mMatWorld.m[3][2] }; }
	void CalcurateOBB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	OBB GetOBB() { return mOBB; }
private:
	OBB mOBB;
};

