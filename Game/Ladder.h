#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <d3d12.h>

struct AABB;

class Ladder : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	void SetModel(Model* model) { mModel = model; }
	void SetTexture(Texture* texture) { mTexture = texture; }
private:
	AABB mAABB;
};

