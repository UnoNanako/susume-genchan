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

	mTerrainTexture = new Texture();
	mTerrainTexture->Create(mDxCommon, "resources/trainTexture/M_train_2_BaseColor.png");

	mTrainModel = new Model();
	mTrainModel->Create(mDxCommon, "resources", "train.obj");
	mTrainModel->SetTexture(mTerrainTexture);
	mTrainTransform = { {0.1f,0.1f,0.1f},{0.0f,0.0f,0.0f},{0.0f,-10.0f,0.0f} };

	//とりあえずの地形
	mTerrainModel.resize(mTERRAIN_MAX);
	for (uint32_t i = 0; i < mTerrainModel.size(); ++i) {
		mTerrainModel[i] = new Model();
		mTerrainModel[i]->Create(mDxCommon, "resources", "floor.obj");
	}
	mTerrainTransform.resize(mTERRAIN_MAX);
	for (uint32_t i = 0; i < mTerrainTransform.size(); ++i) {
		mTerrainTransform[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	}
	//床
	mTerrainTransform[0] = { {100.0f,1.0f,100.0f} ,{0.0f,0.0f,0.0f} ,{0.0f,-10.0f,0.0f} };
	//壁1(右)
	mTerrainTransform[1] = { {1.0f,2.0f,100.0f},{0.0f,0.0f,0.0f},{100.0f,-7.0f,0.0f} };
	//壁2(左)
	mTerrainTransform[2] = { {1.0f,2.0f,100.0f},{0.0f,0.0f,0.0f},{-100.0f,-7.0f,0.0f} };
	//壁3(奥)
	mTerrainTransform[3] = { {100.0f,2.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,-7.0f,100.0f} };
	//壁4(手前)
	mTerrainTransform[4] = { {100.0f,2.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,-7.0f,-100.0f} };

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
