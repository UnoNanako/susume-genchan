#pragma once
#include "Transform.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;

class Camera
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Bind(ID3D12GraphicsCommandList* commandList);
	void SetTransform(Transform transform) { this->transform = transform; }
	Transform GetTransform() { return transform; }
	Matrix4x4 GetViewMatrix() { return viewMatrix; }
	Matrix4x4 GetProjectionMatrix() { return projectionMatrix; }
private:
	Transform transform;
	Matrix4x4 matrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;

	Vector3* cameraData = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraRresource;
};

