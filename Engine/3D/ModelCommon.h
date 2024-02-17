#pragma once
#include <d3d12.h>
#include <wrl.h>

class DirectXCommon;

class ModelCommon
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Bind(DirectXCommon* dxCommon);
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
};

