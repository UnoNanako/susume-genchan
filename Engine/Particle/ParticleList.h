#pragma once
#include <d3d12.h>
#include <string>
#include "Transform.h"
#include "Particle.h"
#include <wrl.h>
#include <random>
#include <numbers>
#include <list>

class DirectXCommon;
struct ParticleForGPU;
class Camera;
class Texture;
struct VertexData;
struct Material;

const uint32_t kNumMaxInstance = 100; //インスタンス数

class ParticleList
{
public:
	~ParticleList();
	void Create(DirectXCommon* dxCommon, const std::string& directoryPath);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	void DrawImGui();
	Particle MakeNewParticle(const Vector3& translate);
	std::list<Particle> Emit(const Emitter& emitter);
	/// <summary>
	/// アクセッサ
	/// </summary>
	//エミッタ
	void SetEmitTransform(Transform transform) { mEmitter.transform = transform; }
	void SetEmitScale(Vector3 scale) { mEmitter.transform.scale = scale; }
	void SetEmitRotate(Vector3 rotate) { mEmitter.transform.rotate = rotate; }
	void SetEmitTranslate(Vector3 translate) { mEmitter.transform.translate = translate; }
	void SetFrequency(float frequency) { mEmitter.frequency = frequency; }
	
	//パーティクル
	void SetParticleTransform(Transform transform) { mTransformInit = transform; }
	void SetParticleScale(Vector3 scale) { mTransformInit.scale = scale; }
	void SetParticleRotate(Vector3 rotate) { mTransformInit.rotate = rotate; }
	void SetParticleTranslate(Vector3 translate) { mTransformInit.translate = translate; }
	void SetTranslateMin(Vector3 translateMin) { mTranslateMin = translateMin; }
	void SetTranslateMax(Vector3 translateMax) { mTranslateMax = translateMax; }
	void SetVelocityMin(Vector3 velocityMin) { mVelocityMin = velocityMin; }
	void SetVelocityMax(Vector3 velocityMax) { mVelocityMax = velocityMax; }
	void SetColorMin(Vector3 colorMin) { mColorMin = colorMin; }
	void SetColorMax(Vector3 colorMax) { mColorMax = colorMax; }
	void SetLifeTImeMin(float lifeTimeMin) { mLifeTimeMin = lifeTimeMin; }
	void SetLifeTimeMax(float lifeTimeMax) { mLifeTimeMax = lifeTimeMax; }
	void SetIsPlaying(bool isPlaying) { mIsPlaying = isPlaying; }
	bool GetIsPlaying() { return mIsPlaying; }
	Transform GetTransformInit() { return mTransformInit; }

private:
	const float kDeltaTime = 1.0f / 60.0f; //tを定義。とりあえず60fps
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResourse;
	ParticleForGPU* instancingData;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	VertexData* vertexData;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData;
	Texture* texture;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
	//Particle mParticles[kNumMaxInstance];
	std::list<Particle> mParticles;
	uint32_t mNumInstance = 0; //描画すべきインスタンス数
	float mAlpha; //透明度
	Emitter mEmitter{};
	Transform mTransformInit; //パーティクルセッター用のメンバ
	bool mIsPlaying; //パーティクルが出ているか
	//ランダム範囲の最小、最大
	Vector3 mTranslateMin;
	Vector3 mTranslateMax;
	Vector3 mVelocityMin;
	Vector3 mVelocityMax;
	Vector3 mColorMin;
	Vector3 mColorMax;
	float mLifeTimeMin;
	float mLifeTimeMax;
};

