#pragma once
#include <numbers>
#include <d3d12.h>
#include <wrl.h>
#include "Transform.h"

class DirectXCommon;
struct VertexData;
struct Material;
class Camera;

class SphereModel
{
public:
	void Create(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList *commandList, Camera* camera);

	void SetTranslate(Vector3 translate) { transform.translate = translate; }

private:
	const uint32_t kSubdivision = 16; //分割数
	const uint32_t kVertexCount = kSubdivision * kSubdivision * 6;//球体頂点数
	const float kPi = std::numbers::pi_v<float>;
	const float kLonEvery = (2 * kPi) / float(kSubdivision); //経度分割1つ分の角度
	const float kLatEvery = kPi / float(kSubdivision); //緯度分割1つ分の角度
	Transform transform;
	Transform cameraTransform;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData;
	TransformationMatrix* wvpData;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
};

