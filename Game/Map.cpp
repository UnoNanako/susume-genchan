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
	//床
	mTerrainTransform[0] = { {60.0f,1.0f,60.0f} ,{0.0f,0.0f,0.0f} ,{0.0f,-10.0f,0.0f} };
	//壁1
	mTerrainTransform[1] = { {5.0f,5.0f,20.0f},{0.0f,0.0f,0.0f},{-17.5f,-7.5f,-20.0f} };
	//壁2
	mTerrainTransform[2] = { {5.0f,5.0f,20.0f},{0.0f,0.0f,0.0f},{-17.5f,-7.5f,0.0f} };
	//壁3
	mTerrainTransform[3] = { {5.0f,5.0f,15.0f},{0.0f,0.0f,0.0f},{-22.5f,-7.5f,12.5f} };
	//壁4
	mTerrainTransform[4] = { {10.0f,5.0f,5.0f},{0.0f,0.0f,0.0f},{0.0f,-7.5f,22.5f} };
	//壁5
	mTerrainTransform[5] = { {5.0f,5.0f,5.0f},{0.0f,0.0f,0.0f},{2.5f,-7.5f,17.5f} };
	//壁6
	mTerrainTransform[6] = { {10.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{0.0f,-7.5f,10.0f} };
	//壁7
	mTerrainTransform[7] = { {10.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{-5.0f,-7.5f,0.0f} };
	//壁8
	mTerrainTransform[8] = { {5.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{-7.5f,-7.5f,-10.0f} };
	//壁9
	mTerrainTransform[9] = { {15.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{-2.5f,-7.5f,-20.0f} };
	//壁10
	mTerrainTransform[10] = { {10.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{15.0f,-7.5f,-20.0f} };
	//壁11
	mTerrainTransform[11] = { {20.0f,5.0f,5.0f},{0.0f,0.0f,0.0f},{15.0f,-7.5f,-2.5f} };
	//壁12
	mTerrainTransform[12] = { {5.0f,5.0f,10.0f},{0.0f,0.0f,0.0f},{27.5f,-7.5f,10.0f} };
	//壁13
	mTerrainTransform[13] = { {5.0f,10.0f,10.0f},{0.0f,0.0f,0.0f},{27.5f,-5.0f,20.0f} };

	mTerrainAABB.resize(mTERRAIN_MAX);
	//床、壁のmin,maxを求める
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainAABB[i] = CalcurateAABB(mTerrainTransform[i].translate, mTerrainTransform[i].scale);
	}

	//mAABBInvisible
	//mAABBInvisibleWall.resize(4);
	//float sizeX = 50.0f;
	//float sizeZ = 150.0f;
	//mAABBInvisibleWall[0] = { //playerから見て正面
	//	{mTrainTransform.translate.x - sizeX, mTrainTransform.translate.y - 100.0f , mTrainTransform.translate.z + sizeZ - 20.0f},
	//	{mTrainTransform.translate.x + sizeX, mTrainTransform.translate.y + 100.0f , mTrainTransform.translate.z + sizeZ + 20.0f}
	//};																		
	//mAABBInvisibleWall[1] = { //playerから見て後ろ						   
	//	{mTrainTransform.translate.x - sizeX, mTrainTransform.translate.y - 100.0f, mTrainTransform.translate.z - sizeZ - 20.0f},
	//	{mTrainTransform.translate.x + sizeX, mTrainTransform.translate.y + 100.0f, mTrainTransform.translate.z - sizeZ + 20.0f}
	//};																		
	//mAABBInvisibleWall[2] = { //playerから見て左								
	//	{mTrainTransform.translate.x - sizeX - 40.0f ,mTrainTransform.translate.y - 100.0f, mTrainTransform.translate.z - sizeZ},
	//	{mTrainTransform.translate.x - sizeX + 40.0f,mTrainTransform.translate.y + 100.0f, mTrainTransform.translate.z + sizeZ }
	//};																		
	//mAABBInvisibleWall[3] = { //playerから見て右								
	//	{mTrainTransform.translate.x + sizeX - 40.0f, mTrainTransform.translate.y - 100.0f, mTrainTransform.translate.z - sizeZ},
	//	{mTrainTransform.translate.x + sizeX + 40.0f, mTrainTransform.translate.y + 100.0f, mTrainTransform.translate.z + sizeZ}
	//};
}

void Map::Update()
{
	ImGui::Begin("Debug");
	ImGui::DragFloat3("terrainRotation", &mTerrainTransform[1].rotate.x, 0.01f);
	ImGui::End();
}

void Map::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	//mTerrainTexture->Bind(commandList);
	//mTerrainTexture->Bind(commandList);
	//mModel->Draw(commandList,camera, mTransform); 
	//mTrainModel->Draw(commandList, camera, mTrainTransform);
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainModel[i]->Draw(commandList, camera, mTerrainTransform[i]);
	}
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
