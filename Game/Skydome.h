#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"


class Skydome : public GameObject{
public:
	~Skydome();
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commansList,Camera* camera);

private:
};

