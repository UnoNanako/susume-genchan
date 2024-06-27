#pragma once
#include "GameObject.h"
#include <d3d12.h>
#include <memory>

class AABB;
class ParticleList;

class Star : public GameObject{
public:
	~Star();
	void Initialize(DirectXCommon* dxCommon)override;
	void Update()override;
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)override;
	void ParticleDraw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	AABB GetAABB() { return mAABB; }

private:
	AABB mAABB;
	std::unique_ptr<ParticleList> mParticle;
};

