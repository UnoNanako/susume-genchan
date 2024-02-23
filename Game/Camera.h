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
	void SetTransform(Transform mTransform) { this->mTransform = mTransform; }
	Transform GetTransform() { return mTransform; }
	Matrix4x4 GetViewMatrix() { return mViewMatrix; }
	Matrix4x4 GetProjectionMatrix() { return mProjectionMatrix; }
private:
	Transform mTransform;
	Matrix4x4 mMatrix;
	Matrix4x4 mViewMatrix;
	Matrix4x4 mProjectionMatrix;

	Vector3* cameraData = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource;
};

