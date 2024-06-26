#pragma once
#include "Transform.h"
#include "Engine/GameObject.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class DirectXCommon;
class Texture;
struct AABB;
class ParticleList;
class Sprite;

class Gem : public GameObject{
public:
	~Gem();
	void Initialize(DirectXCommon* dxCommon, Vector3 translate);
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	void SpriteDraw(ID3D12GraphicsCommandList* commandList);
	void ParticleDraw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	AABB GetAABB() { return mAABB; }
	bool GetIsHit() { return mIsHit; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }

private:
	std::unique_ptr<Texture> mGetColor;
	AABB mAABB;
	bool mIsHit = false;
	std::unique_ptr<ParticleList> mParticle;
	std::unique_ptr<Sprite> mGemSprite;
	std::unique_ptr<Sprite> mNotGemSprite;
};

