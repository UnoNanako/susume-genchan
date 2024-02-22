#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include "Transform.h"
#include <vector>

class DirectXCommon;
class Camera;
class Model;

class RotateEnemy : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon)  override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera) override;

	void SetTranslate(const Vector3& translate) { mTransform.translate = translate; }
private:
};

