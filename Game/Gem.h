#pragma once
#include "Transform.h"
#include "Engine/GameObject.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;
class Texture;
struct AABB;

class Gem : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minとmaxを求める関数
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	AABB GetAABB() { return mAABB; }

private:
	Texture* mGetColor; //ユニークポインタにする
	AABB mAABB;
};

