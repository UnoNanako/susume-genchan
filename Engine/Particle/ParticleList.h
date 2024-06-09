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
	void Create(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera, const Transform& mTransform);
	Particle MakeNewParticle(const Vector3& translate);
	std::list<Particle> Emit(const Emitter& emitter);
	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetEmitTransform(Transform transform) { mEmitter.transform = transform; }
	void SetParticleScale(Vector3 scale) { mScaleInit = scale; }
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
	Vector3 mScaleInit; //セットスケール用のメンバ
};

