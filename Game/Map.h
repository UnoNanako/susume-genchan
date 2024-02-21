#pragma once
#include <d3d12.h>
#include "Transform.h"
#include "GameObject.h"
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>

class Texture;
class Model;
struct AABB;

const std::string pathToJSON("./resources/json/map.json");

class Map : public GameObject
{
public:
	void Create(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);

	//translateとscaleを渡すとmin,maxを求めてくれる関数
	AABB CalcurateAABB(const Vector3& translate, const Vector3& scale);
	
	std::vector<Model*> GetTerrainModel() { return mTerrainModel; }
	std::vector<Transform> GetTerrainTransform() { return mTerrainTransform; }
	std::vector<AABB> GetTerrainAABB() { return mTerrainAABB; }

	//vector<AABB> GetAABBInvisible() { return mAABBInvisibleWall; }
private:
	//地面
	Texture* mTerrainTexture;

	//壁(目に見えない)
	//vector<AABB> mAABBInvisibleWall;

	//電車(のちに削除)
	Model* mTrainModel;
	Texture* mTrainTexture;
	Transform mTrainTransform;

	//とりあえずの地形
	std::vector<Model*> mTerrainModel; //壁や床など
	std::vector<Transform> mTerrainTransform;
	const int mTERRAIN_MAX = 15; //地形の数
	std::vector<AABB> mTerrainAABB;
};

