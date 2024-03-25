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

const std::string pathToJSON("./resources/Json/map.json");
const std::string pathToAABBJSON("./resources/Json/mapAABB.json");

class Map : public GameObject
{
public:
	void Create(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);
	void Finalize();
	//translateとscaleを渡すとmin,maxを求めてくれる関数
	AABB CalcurateAABB(const Vector3& translate, const Vector3& scale);
	
	std::vector<Model*> GetTerrainModel() { return mTerrainModel; }
	std::vector<Transform> GetTerrainTransform() { return mTerrainTransform; }
	std::vector<AABB> GetTerrainAABB() { return mTerrainAABB; }
	std::vector<AABB> GetInvisibleAABB() { return mInvisibleAABB; }

private:
	//地形
	Texture* mTerrainTexture;
	std::vector<Model*> mTerrainModel; //壁や床など
	std::vector<Transform> mTerrainTransform;
	std::vector<AABB> mTerrainAABB;

	//見えない壁(当たり判定用)
	std::vector<Model*> mInvisibleAABBModel; //見えない壁のvector
	std::vector<Transform> mInvisibleAABBTransform;
	std::vector<AABB> mInvisibleAABB; 
};

