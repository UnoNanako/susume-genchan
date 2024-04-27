#include "Map.h"
#include "3D/Model.h"
#include "Transform.h"
#include "PlayerCamera.h"
#include "VertexData.h"
#include "2D/Texture.h"
#include "Math/MyMath.h"
#include "Engine/ImGuiManager.h"
#include "Block.h"
#include <format>

void Map::Create(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;

	mTerrainTexture = new Texture();
	mTerrainTexture->Create(mDxCommon, "resources/Model/Blocks/Grass/Blocks_PixelArt.png");

	//jsonファイル読み込み
	//読み込むファイルの名前を作成
	std::ifstream ifs(pathToJSON.c_str());
	if (ifs.good()) {
		nlohmann::json mJson;
		ifs >> mJson;
		int terrainCount = mJson["TerrainCount"].get<int>();
		//読み込んだデータをそれぞれの変数に代入する
		for (uint32_t i = 0; i < terrainCount; ++i) {
			nlohmann::json& terrainData = mJson[std::format("Terrain{}", i)]; //これあとで理解
			Model* newModel;

			//モデルの番号を読み取る
			ModelIndex modelIndex = terrainData["ModelIndex"].get<ModelIndex>();

			Block* block = new Block();
			block->mName = terrainData["Name"].get<std::string>();

			//特定の番号に対応するモデルとテクスチャを設定する
			switch (modelIndex) {
			case FLOOR:
				//床のモデルを読み込む
				block->mModel = new Model();
				block->mModel->SetModelIndex(0);
				block->mModel->Create(mDxCommon, "resources/Model/Floor/Floor", "floor.obj");
				break;

			case GRASS:
				//草のモデルを読み込む
				block->mModel = new Model();
				block->mModel->SetModelIndex(1);
				block->mModel->Create(mDxCommon, "resources/Model/Blocks/Grass", "grass.obj");
				block->mModel->SetTexture(mTerrainTexture);
				break;

			case Wall:
				//壁のモデルを読み込む
				block->mModel = new Model();
				block->mModel->SetModelIndex(2);
				block->mModel->Create(mDxCommon, "resources/Model/Blocks/Wall", "wall.obj");
				break;
			}

			///jsonの初期化に使用
			/*newModel = new Model();
			newModel->SetModelIndex(0);
			newModel->Create(mDxCommon, "resources/Model/Blocks/Grass", "grass.obj");
			newModel->SetTexture(mTerrainTexture);
			mTerrainModel.emplace_back(newModel);*/

			//各地形のTransformをベクターに追加。これにより、あとで地形の位置、スケール、回転などを取得できるようになる
			block->mTransform.translate.x = terrainData["Position"][0].get<float>();
			block->mTransform.translate.y = terrainData["Position"][1].get<float>();
			block->mTransform.translate.z = terrainData["Position"][2].get<float>();
			block->mTransform.scale.x = terrainData["Scale"][0].get<float>();
			block->mTransform.scale.y = terrainData["Scale"][1].get<float>();
			block->mTransform.scale.z = terrainData["Scale"][2].get<float>();
			block->mTransform.rotate.x = 0.0f;
			block->mTransform.rotate.y = 0.0f;
			block->mTransform.rotate.z = 0.0f;
			block->mAABB.max.x = terrainData["Max"][0].get<float>();
			block->mAABB.max.y = terrainData["Max"][1].get<float>();
			block->mAABB.max.z = terrainData["Max"][2].get<float>();
			block->mAABB.min.x = terrainData["Min"][0].get<float>();
			block->mAABB.min.y = terrainData["Min"][1].get<float>();
			block->mAABB.min.z = terrainData["Min"][2].get<float>();
			mBlock.emplace_back(block);
		}
	}
}

void Map::Update()
{
	Block* block = new Block();
	ImGui::Begin("Map");
	if (ImGui::Button("Create")) {
		block->mModel = new Model();
		block->mModel->Create(mDxCommon, "resources/Model/Blocks/Grass", "grass.obj");
		block->mModel->SetTexture(mTerrainTexture);
		mBlock.emplace_back(block);
		Transform newTransform;
		newTransform.scale.x = 5.0f;
		newTransform.scale.y = 5.0f;
		newTransform.scale.z = 5.0f;
		newTransform.rotate.x = 0.0f;
		newTransform.rotate.y = 0.0f;
		newTransform.rotate.z = 0.0f;
	
		AABB newAABB;
		newAABB = CalcurateAABB(newTransform.translate, newTransform.scale);
	}
	int i = 0;
	for (auto iter = mBlock.begin(); iter != mBlock.end();) {
		ImGui::InputText(std::format("Name##{}",i).c_str(), &(*iter)->mName);
		ImGui::DragFloat3(std::format("terrainTransform{}", i).c_str(), &(*iter)->mTransform.translate.x, 0.01f);
		ImGui::DragFloat3(std::format("terrainScale{}", i).c_str(), &(*iter)->mTransform.scale.x, 0.01f);
		ImGui::DragFloat3(std::format("max##{}", i).c_str(), &(*iter)->mAABB.max.x);
		ImGui::DragFloat3(std::format("min##{}", i).c_str(), &(*iter)->mAABB.min.x);
		if (Combo(std::format("ModelIndex##{}", i).c_str(), (*iter)->mModelIndex)) {
			switch ((*iter)->mModelIndex) {
			case FLOOR:
				//床のモデルを読み込む
				(*iter)->mModel = new Model();
				(*iter)->mModel->SetModelIndex(0);
				(*iter)->mModel->Create(mDxCommon, "resources/Model/Floor/Floor", "floor.obj");
				break;

			case GRASS:
				//草のモデルを読み込む
				(*iter)->mModel = new Model();
				(*iter)->mModel->SetModelIndex(1);
				(*iter)->mModel->Create(mDxCommon, "resources/Model/Blocks/Grass", "grass.obj");
				(*iter)->mModel->SetTexture(mTerrainTexture);
				break;

			case Wall:
				//壁のモデルを読み込む
				(*iter)->mModel = new Model();
				(*iter)->mModel->SetModelIndex(2);
				(*iter)->mModel->Create(mDxCommon, "resources/Model/Blocks/Wall", "wall.obj");
				break;
			}
		}
		if (ImGui::Button(std::format("Erase##{}",i).c_str())) {
			iter = mBlock.erase(iter);
		}
		else {
			++iter;
		}
		++i;
		ImGui::Separator();
	}
	ImGui::DragFloat3("floorScale", &mBlock[0]->mTransform.scale.x, 0.01f);
	ImGui::End();
	for (uint32_t i = 0; i < mBlock.size(); ++i) {
		mBlock[i]->mTransform.UpdateMatrix();
	}
}

void Map::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	for (uint32_t i = 0; i < mBlock.size(); ++i) {
		mBlock[i]->mModel->Draw(commandList, camera, mBlock[i]->mTransform);
	}
}

void Map::Finalize()
{
	//mapJson書き込み
	std::ofstream file(pathToJSON.c_str());
	nlohmann::json data;
	data["TerrainCount"] = mBlock.size();
	for (uint32_t i = 0; i < mBlock.size(); ++i) {
		nlohmann::json& terrainData = data[std::format("Terrain{}", i)]; //これあとで理解
		terrainData["Position"].push_back(mBlock[i]->mTransform.translate.x);
		terrainData["Position"].push_back(mBlock[i]->mTransform.translate.y);
		terrainData["Position"].push_back(mBlock[i]->mTransform.translate.z);
		terrainData["Scale"].push_back(mBlock[i]->mTransform.scale.x);
		terrainData["Scale"].push_back(mBlock[i]->mTransform.scale.y);
		terrainData["Scale"].push_back(mBlock[i]->mTransform.scale.z);
		terrainData["ModelIndex"] = mBlock[i]->mModel->GetModelIndex();
		terrainData["Name"] = mBlock[i]->mName;
		terrainData["Min"].push_back(mBlock[i]->mAABB.min.x);
		terrainData["Min"].push_back(mBlock[i]->mAABB.min.y);
		terrainData["Min"].push_back(mBlock[i]->mAABB.min.z);
		terrainData["Max"].push_back(mBlock[i]->mAABB.max.x);
		terrainData["Max"].push_back(mBlock[i]->mAABB.max.y);
		terrainData["Max"].push_back(mBlock[i]->mAABB.max.z);
	}
	file << data.dump(4) << std::endl;
}

AABB Map::CalcurateAABB(const Vector3& translate, const Vector3& scale)
{
	AABB ret;
	ret.min = {
		{ translate.x - (scale.x / 2.0f) },
		{ translate.y - (scale.y / 2.0f) },
		{ translate.z - (scale.z / 2.0f) }
	};
	ret.max = {
		{ translate.x + (scale.x / 2.0f) },
		{ translate.y + (scale.y / 2.0f) },
		{ translate.z + (scale.z / 2.0f) }
	};
	return ret;
}
