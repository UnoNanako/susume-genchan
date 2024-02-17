#pragma once
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include "Transform.h"

class DirectXCommon;
class Texture;
struct Material;
struct TransformationMatrix;

//スプライト
class Sprite
{
public:
	void Create(DirectXCommon* dxCommon, const std::string& filePath);
	void Update();
	void Draw(ID3D12GraphicsCommandList *commandList);
	void SetTransform(Transform transform) { this->transform = transform; }
	~Sprite();
private:
	Texture* texture;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	uint32_t* indexData;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData;
	Transform transform;
};

