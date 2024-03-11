#pragma once
#include "GameObject.h"
#include "Transform.h"
#include<d3d12.h>

class MoveFloor : public GameObject
{
protected:
	virtual void Initialize(DirectXCommon* dxCommon);
	virtual void Update();
	virtual void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);

	bool mIsMove; //動いているか
};

