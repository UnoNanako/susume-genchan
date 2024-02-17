#pragma once
#include <d3d12.h>
#include <wrl.h>

class DirectXCommon;

class ParticleCommon
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Bind(DirectXCommon* dxCommon);
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> particleRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> particleGraphicsPipelineState;
};

