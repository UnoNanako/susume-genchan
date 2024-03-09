#include "Map.h"
#include "3D/Model.h"
#include "Transform.h"
#include "PlayerCamera.h"
#include "VertexData.h"
#include "2D/Texture.h"
#include "Math/MyMath.h"
#include "externals/imgui/imgui.h"

void Map::Create(DirectXCommon* dxCommon)
{
	mDxCommon = dxCommon;

	mTerrainTexture = new Texture();
	mTerrainTexture->Create(mDxCommon, "resources/blocks/grass/Blocks_PixelArt.png");

	mModel = new Model();
	mModel->Create(mDxCommon, "resources", "terrain.obj");
	mModel->SetTexture(mTerrainTexture);
	mTransform = { {1.0f,1.0f,1.0f} ,{0.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f} };

	//jsonファイル読み込み
	//読み込むファイルの名前を作成
	std::ifstream ifs(pathToJSON.c_str());
	if (ifs.good()) {
		nlohmann::json mJson;
		ifs >> mJson;
		int terrainCount = mJson["TerrainCount"].get<int>();
		//読み込んだデータをそれぞれの変数に代入する
		for (uint32_t i = 0; i < terrainCount; ++i) {
			Model* newModel;
			newModel = new Model();
			newModel->Create(mDxCommon, "resources/blocks/grass", "grass.obj");
			newModel->SetTexture(mTerrainTexture);
			mTerrainModel.emplace_back(newModel);

			Transform newTransform;

			nlohmann::json& terrainData = mJson[std::format("Terrain{}", i)]; //これあとで理解
			newTransform.translate.x = terrainData["Position"][0].get<float>();
			newTransform.translate.y = terrainData["Position"][1].get<float>();
			newTransform.translate.z = terrainData["Position"][2].get<float>();
			newTransform.scale.x = terrainData["Scale"][0].get<float>();
			newTransform.scale.y = terrainData["Scale"][1].get<float>();
			newTransform.scale.z = terrainData["Scale"][2].get<float>();
			newTransform.rotate.x = 0.0f;
			newTransform.rotate.y = 0.0f;
			newTransform.rotate.z = 0.0f;
			mTerrainTransform.emplace_back(newTransform);

			AABB newAABB;
			newAABB = CalcurateAABB(newTransform.translate, newTransform.scale);
			mTerrainAABB.emplace_back(newAABB);
		}
	}
	//AABBjsonファイル読み込み
	//読み込むファイルの名前を作成
	//std::ifstream ifstream(pathToAABBJSON.c_str());
	//if (ifstream.good()) {
	//	nlohmann::json aabbJson;
	//	ifstream >> aabbJson;
	//	int invisibleModelCount = aabbJson["invisibleModelCount"].get<int>();
	//	//読み込んだデータをそれぞれの変数に代入する
	//	for (uint32_t i = 0; i < invisibleModelCount; ++i) {
	//		Model* newModel;
	//		newModel = new Model();
	//		newModel->Create(mDxCommon, "resources", "floor.obj");
	//		mInvisibleAABBModel.emplace_back(newModel);

	//		Transform newTransform;

	//		nlohmann::json& invisibleData = aabbJson[std::format("invisible{}", i)];
	//		newTransform.translate.x = invisibleData["Position"][0].get<float>();
	//		newTransform.translate.y = invisibleData["Position"][1].get<float>();
	//		newTransform.translate.z = invisibleData["Position"][2].get<float>();
	//		newTransform.scale.x = invisibleData["Scale"][0].get<float>();
	//		newTransform.scale.y = invisibleData["Scale"][1].get<float>();
	//		newTransform.scale.z = invisibleData["Scale"][2].get<float>();
	//		newTransform.rotate.x = 0.0f;
	//		newTransform.rotate.y = 0.0f;
	//		newTransform.rotate.z = 0.0f;
	//		mInvisibleAABBTransform.emplace_back(newTransform);

	//		AABB newAABB;
	//		newAABB = CalcurateAABB(newTransform.translate, newTransform.scale);
	//		mInvisibleAABB.emplace_back(newAABB);
	//	}
	//}
}

void Map::Update()
{
	ImGui::Begin("Map");
	if (ImGui::Button("Create")) {
		Model* newModel;
		newModel = new Model();
		newModel->Create(mDxCommon, "resources/blocks/grass", "grass.obj");
		newModel->SetTexture(mTerrainTexture);
		mTerrainModel.emplace_back(newModel);
		Transform newTransform;
		newTransform.scale.x = 5.0f;
		newTransform.scale.y = 5.0f;
		newTransform.scale.z = 5.0f;
		newTransform.rotate.x = 0.0f;
		newTransform.rotate.y = 0.0f;
		newTransform.rotate.z = 0.0f;
		mTerrainTransform.emplace_back(newTransform);
		AABB newAABB;
		newAABB = CalcurateAABB(newTransform.translate, newTransform.scale);
		mTerrainAABB.emplace_back(newAABB);
	}
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		ImGui::DragFloat3(std::format("terrainTransform{}", i).c_str(), &mTerrainTransform[i].translate.x, 0.01f);
	}
	ImGui::DragFloat3("floorScale", &mTerrainTransform[0].scale.x, 0.01f);
	ImGui::End();
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainTransform[i].UpdateMatrix();
	}
}

void Map::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainModel[i]->Draw(commandList, camera, mTerrainTransform[i]);
	}
}

void Map::Finalize()
{
	//mapJson書き込み
	std::ofstream file(pathToJSON.c_str());
	nlohmann::json data;
	data["TerrainCount"] = mTerrainModel.size();
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		nlohmann::json& terrainData = data[std::format("Terrain{}", i)]; //これあとで理解
		terrainData["Position"].push_back(mTerrainTransform[i].translate.x);
		terrainData["Position"].push_back(mTerrainTransform[i].translate.y);
		terrainData["Position"].push_back(mTerrainTransform[i].translate.z);
		terrainData["Scale"].push_back(mTerrainTransform[i].scale.x);
		terrainData["Scale"].push_back(mTerrainTransform[i].scale.y);
		terrainData["Scale"].push_back(mTerrainTransform[i].scale.z);
	}
	file << data.dump(4) << std::endl;

	//aabbJson書き込み
	/*std::ofstream aabbFile(pathToAABBJSON.c_str());
	nlohmann::json aabbData;
	aabbData["invisibleModelCount"] = mInvisibleAABBModel.size();
	for (uint32_t i = 0; i < mInvisibleAABBModel.size(); ++i) {
		nlohmann::json& data = aabbData[std::format("invisible{}", i)];
		data["Position"].push_back(mInvisibleAABBTransform[i].translate.x);
		data["Position"].push_back(mInvisibleAABBTransform[i].translate.y);
		data["Position"].push_back(mInvisibleAABBTransform[i].translate.z);
		data["Scale"].push_back(mInvisibleAABBTransform[i].scale.x);
		data["Scale"].push_back(mInvisibleAABBTransform[i].scale.y);
		data["Scale"].push_back(mInvisibleAABBTransform[i].scale.z);
	}
	aabbFile << aabbData.dump(4) << std::endl;*/
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
