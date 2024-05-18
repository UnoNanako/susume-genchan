#include "DirectXCommon.h"
#include "Material.h"
#include "Model.h"
#include "2D/Texture.h"
#include "Transform.h"
#include "VertexData.h"
#include "WinApiManager.h"
#include <cassert>
#include <fstream>
#include <numbers>
#include <sstream>
#include <Windows.h>
#include "PlayerCamera.h"
#include "ImGuiManager.h"
#include "Math/Quaternion.h"

using namespace std;

Model::~Model()
{
}

void Model::Create(DirectXCommon* dxCommon, const std::string& directoryPath,const std::string& filePath)
{
	Load(directoryPath,filePath);
	//頂点
	vertexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData) * mModelData.vertices.size());
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * mModelData.vertices.size();
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, mModelData.vertices.data(), sizeof(VertexData)* mModelData.vertices.size());// 頂点データをリソースにコピー

	//色
	//Sprite用のマテリアルリソースを作る
	materialResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	//SpriteはLightingしないのでfalseを設定する
	materialData->enableLighting = true;
	materialData->shininess = 100.0f;

	//Texture
	texture = new Texture();
	texture->Create(dxCommon, mModelData.material.textureFilePath);

	//Transform
	transformationMatrixResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	//単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();
	//CPUで動かす用のTransformWVP作る
	mTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Model::Update()
{
	//Animationを再生する
	float animationTime = 0.0f;
	//時刻を進めて、指定した時刻の各種データを取得し。localMatrixを生成する
	animationTime += 1.0f / 60.0f; //時刻を進める、1/60で固定してあるが、計測した時間を使って可変フレーム対応する方が望ましい
	animationTime = std::fmod(animationTime, mAnimation.duration); //最後まで言ったらリピート再生。リピートしなくても別にいい
	NodeAnimation& rootNodeAnimation = mAnimation.nodeAnimations[mModelData.rootNode.name]; //rootNodeのAnimationを取得
	
}

void Model::Draw(ID3D12GraphicsCommandList* commandList,Camera* camera,const Transform &mTransform)
{
	//const float kPi = std::numbers::pi_v<float>;
	//Model用のWorldViewProjectionMatrixをつくる
	Matrix4x4 worldMatrix = mTransform.mMatWorld;//MakeAffineMatrix(mTransform.scale, mTransform.rotate, mTransform.translate);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
	transformationMatrixData->WVP = Multiply(mModelData.rootNode.localMatrix,worldViewProjectionMatrix);
	transformationMatrixData->World = Multiply(mModelData.rootNode.localMatrix,worldMatrix);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	texture->Bind(commandList);
	commandList->DrawInstanced(UINT(mModelData.vertices.size()), 1, 0, 0);
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	//1. 中で必要となる変数の宣言
	MaterialData materialData; //構築するMaterialData
	materialData.textureFilePath = "resources/default.png";
	std::string line; //ファイルから呼んだ1行を格納するもの

	//2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
	assert(file.is_open()); //とりあえず開けなかったら止める

	//3. 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::stringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	//4. MaterialDataを返す
	return materialData;
}

void Model::Load(const std::string& directoryPath, const std::string& filename)
{
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	mModelData.rootNode = ReadNode(scene->mRootNode);
	assert(scene->HasMeshes()); //メッシュがないのは対応しない
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); //法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); //TexcoordがないMeshは今回は非対応
		//ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); //三角形のみのサポート
			//ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };
				//aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				mModelData.vertices.push_back(vertex);
				//materialを解析する
				mModelData.material.textureFilePath = "resources/default.png";
				for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
					aiMaterial* material = scene->mMaterials[materialIndex];
					if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
						aiString textureFilePath;
						material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
						mModelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
					}
				}
			}
		}
	}
}

Node Model::ReadNode(aiNode* node){
	Node ret;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; //nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); //列ベクトル形式を行ベクトル形式に転置
	ret.localMatrix.m[0][0] = aiLocalMatrix[0][0]; 
	ret.localMatrix.m[0][1] = aiLocalMatrix[0][1];
	ret.localMatrix.m[0][2] = aiLocalMatrix[0][2];
	ret.localMatrix.m[0][3] = aiLocalMatrix[0][3];
	ret.localMatrix.m[1][0] = aiLocalMatrix[1][0];
	ret.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	ret.localMatrix.m[1][2] = aiLocalMatrix[1][2];
	ret.localMatrix.m[1][3] = aiLocalMatrix[1][3];
	ret.localMatrix.m[2][0] = aiLocalMatrix[2][0];
	ret.localMatrix.m[2][1] = aiLocalMatrix[2][1];
	ret.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	ret.localMatrix.m[2][3] = aiLocalMatrix[2][3];
	ret.localMatrix.m[3][0] = aiLocalMatrix[3][0];
	ret.localMatrix.m[3][1] = aiLocalMatrix[3][1];
	ret.localMatrix.m[3][2] = aiLocalMatrix[3][2];
	ret.localMatrix.m[3][3] = aiLocalMatrix[3][3];
	ret.name = node->mName.C_Str(); //Node名を格納
	ret.children.resize(node->mNumChildren); //子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		//再帰的に読んで階層構造を作っていく
		ret.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return ret;
}

Animation Model::LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
	Animation animation; //今回作るアニメーション
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0); //アニメーションがない
	aiAnimation* animationAssimp = scene->mAnimations[0]; //最初のアニメーションだけ採用。もちろん複数対応するに越したことはない
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond); //時間の単位を秒に変換
	//assimpでは個々のNodeのAnimationをchannelと読んでいるのでchannelをまわしてNodeAnimationの情報をとってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		//Position
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyfreamVector3 keyfream;
			keyfream.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); //ここも秒に変換
			keyfream.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z }; //右手->左手
			nodeAnimation.translate.keyfream.push_back(keyfream);
		}
		//Scale
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyfreamVector3 keyfream;
			keyfream.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); //秒に変換
			keyfream.value = Vector3(keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z);
			nodeAnimation.scale.keyfream.push_back(keyfream);
		}
		//Rotation
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			Keyfream<Quaternion> keyfream;
			keyfream.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); //秒に変換
			keyfream.value = Quaternion(keyAssimp.mValue.w, keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z);
			nodeAnimation.rotate.keyfream.push_back(keyfream);
		}
	}
	//解析完了
	return animation;
}
