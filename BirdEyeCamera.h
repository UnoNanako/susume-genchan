#pragma once
#include "Transform.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;

class BirdEyeCamera
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Bind(ID3D12GraphicsCommandList* commandList);
private:
	Transform transform;
	Matrix4x4 matrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;

	Vector3* cameraData = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource;
};

