#pragma once
#include "Transform.h"
#include "Engine/GameObject.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;
struct AABB;

class Gem : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commansList, Camera* camera);

	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
};

