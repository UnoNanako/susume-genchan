#pragma once
#include <d3d12.h>
#include <string>
#include "Transform.h"
#include "Material.h"
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include <wrl.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class DirectXCommon;
class Texture;
struct Material;
struct TransformationMatrix;
struct VertexData;
class Camera;
class Quaternion;

//ノード
struct Node {
	Matrix4x4 mLocalMatrix;
	std::string name;
	std::vector<Node> children;
};

//モデルデータ
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};

//キーフレーム
template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template<typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframe;
};

struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Animation {
	float duration; //アニメーション全体の尺（単位は秒）
	//NodeAnimationの集合。Node名でひけるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;
};

class Model
{
public:
	~Model();
	void Create(DirectXCommon* dxCommon, const std::string& directoryPath, const std::string& filePath);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera, const Transform& mTransform);
	//マテリアルデータを読む関数
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//glTFを読む関数
	void Load(const std::string& derectoryPath,const std::string& filename);
	Node ReadNode(aiNode* node);
	//Animationを解析する関数
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);
	//任意の時刻の値を取得する
	//Vector3 Ver
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyfream, float time);
	//Quaternion Ver
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	/// <summary>
	/// アクセッサ
	/// </summary>
	void SetScale(Vector3 scale) { mTransform.scale = scale; }
	void SetTexture(Texture* texture) { this->texture = texture; }
	unsigned int GetModelIndex() { return mModelIndex; }
	void SetModelIndex(unsigned int modelIndex) { mModelIndex = modelIndex; }
	void SetAnimation(Animation animation) { mAnimation = animation; }
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData;
	Transform mTransform;
	ModelData mModelData;
	Texture *texture = nullptr;
	unsigned int mModelIndex;
	Animation mAnimation;
	float mAnimationTime = 0.0f;
	Matrix4x4 mLocalMatrix;
};