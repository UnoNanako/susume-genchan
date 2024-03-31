#pragma once
#include <d3d12.h>
#include <string>
#include "Transform.h"
#include "Material.h"
#include <vector>
#include <Windows.h>
#include <wrl.h>

class DirectXCommon;
class Texture;
struct Material;
struct TransformationMatrix;
struct VertexData;
class Camera;

//モデルデータ
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};

class Model
{
public:
	void Create(DirectXCommon* dxCommon, const std::string& directoryPath, const std::string& filePath);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera, const Transform& mTransform);
	//マテリアルデータを読む関数
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//OBJファイルを読む関数
	void LoadObjFile(const std::string& filePath);
	//glTFを読む関数
	void Load(const std::string& derectoryPath,const std::string& filename);

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetScale(Vector3 scale) { mTransform.scale = scale; }
	void SetTexture(Texture* texture) { this->texture = texture; }
	unsigned int GetModelIndex() { return mModelIndex; }
	void SetModelIndex(unsigned int modelIndex) { mModelIndex = modelIndex; }
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData;
	Transform mTransform;
	ModelData modelData;
	Texture *texture;
	unsigned int mModelIndex;
};