#include "Map.h"
#include "3D/Model.h"
#include "Transform.h"
#include "Camera.h"
#include "VertexData.h"
#include "2D/Texture.h"
#include "Math/MyMath.h"
#include "externals/imgui/imgui.h"

void Map::Create(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;

	mTerrainTexture = new Texture();
	mTerrainTexture->Create(mDxCommon, "resources/grass.png");

	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "terrain.obj");
	mModel->SetTexture(mTerrainTexture);
	mTransform = { {1.0f,1.0f,1.0f} ,{0.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f} };

	mTrainModel = new Model();
	mTrainModel->Create(mDxCommon, "resources", "train.obj");
	mTrainModel->SetTexture(mTerrainTexture);
	mTrainTransform = { {0.1f,0.1f,0.1f},{0.0f,0.0f,0.0f},{0.0f,-10.0f,0.0f} };

	//とりあえずの地形
	mTerrainModel.resize(mTERRAIN_MAX);
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainModel[i] = new Model();
		mTerrainModel[i]->Create(mDxCommon, "resources", "floor.obj");
		mTerrainModel[i]->SetTexture(mTerrainTexture);
	}
	mTerrainTransform.resize(mTERRAIN_MAX);
	for (uint32_t i = 0; i < mTerrainTransform.size(); ++i) {
		mTerrainTransform[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	}
	
	//jsonファイル読み込み
	//読み込むファイルの名前を作成
	std::ifstream ifs(pathToJSON.c_str());
	if (ifs.good()) {
		nlohmann::json mJson;
		ifs >> mJson;

		//読み込んだデータをそれぞれの変数に代入する
		for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
			nlohmann::json& terrainData = mJson[std::format("Terranin{}", i)]; //これあとで理解
			mTerrainTransform[i].translate.x = terrainData["Position"][0].get<float>();
			mTerrainTransform[i].translate.y = terrainData["Position"][1].get<float>();
			mTerrainTransform[i].translate.z = terrainData["Position"][2].get<float>();
			mTerrainTransform[i].scale.x = terrainData["Scale"][0].get<float>();
			mTerrainTransform[i].scale.y = terrainData["Scale"][1].get<float>();
			mTerrainTransform[i].scale.z = terrainData["Scale"][2].get<float>();
		}
	}

	mTerrainAABB.resize(mTERRAIN_MAX);
	//床、壁のmin,maxを求める
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainAABB[i] = CalcurateAABB(mTerrainTransform[i].translate, mTerrainTransform[i].scale);
	}
}

void Map::Update()
{
	ImGui::Begin("Debug");
	ImGui::DragFloat3("terrainTranslate", &mTerrainTransform[0].translate.x, 0.01f);
	ImGui::End();
}

void Map::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainModel[i]->Draw(commandList, camera, mTerrainTransform[i]);
	}
}

void Map::Finalize()
{
	//json書き込み
	std::ofstream file(pathToJSON.c_str());
	nlohmann::json data;
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		nlohmann::json& terrainData = data[std::format("Terranin{}", i)]; //これあとで理解
		terrainData["Position"].push_back(mTerrainTransform[i].translate.x);
		terrainData["Position"].push_back(mTerrainTransform[i].translate.y);
		terrainData["Position"].push_back(mTerrainTransform[i].translate.z);
		terrainData["Scale"].push_back(mTerrainTransform[i].scale.x);
		terrainData["Scale"].push_back(mTerrainTransform[i].scale.y);
		terrainData["Scale"].push_back(mTerrainTransform[i].scale.z);
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
