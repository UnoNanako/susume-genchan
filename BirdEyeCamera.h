#pragma once
#include "Transform.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;

class BirdEyeCamera
{
public:
	BirdEyeCamera();
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Bind(ID3D12GraphicsCommandList* commandList);
private:
	Transform mTransform;
	Matrix4x4 mMatrix;
	Matrix4x4 mViewMatrix;
	Matrix4x4 mProjectionMatrix;

	Vector3* cameraData = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource;
};

