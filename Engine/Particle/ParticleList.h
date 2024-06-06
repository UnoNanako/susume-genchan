#pragma once
#include <d3d12.h>
#include <string>
#include "Transform.h"
#include <wrl.h>
#include "Particle.h"

class DirectXCommon;
struct ParticleForGPU;
class Camera;
class Texture;
struct VertexData;
struct Material;

const uint32_t kNumInstance = 10; //インスタンス数

class ParticleList
{
public:
	~ParticleList();
	void Create(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera, const Transform& mTransform);
	/// <summary>
	/// アクセッサ
	/// </summary>
private:
	const float kDeltaTime = 1.0f / 60.0f;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResourse;
	ParticleForGPU* instancingData;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	VertexData* vertexData;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData;
	Texture* texture;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
	Particle mParticles[kNumInstance];
};

