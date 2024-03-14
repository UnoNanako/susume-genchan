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

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }

private:
	Texture* mGetColor; //ユニークポインタにする
};

