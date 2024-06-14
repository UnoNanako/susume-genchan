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
struct Block;

const std::string pathToJSON("./resources/Json/map.json");
const std::string pathToAABBJSON("./resources/Json/mapAABB.json");

class Map : public GameObject{
public:
	//モデル、テクスチャの種類を管理するenum(マップチップのアレ)
	enum ModelIndex : unsigned int {
		FLOOR, //0
		GRASS, //1
		Wall, //2
		HOUSE //3
	};

	void Create(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);
	void Finalize();
	//translateとscaleを渡すとmin,maxを求めてくれる関数
	AABB CalculateAABB(const Vector3& translate, const Vector3& scale);
	
	std::vector<Block*> GetBlock() { return mBlock; }
	std::vector<AABB> GetInvisibleAABB() { return mInvisibleAABB; }

private:
	//地形
	Texture* mTerrainTexture;

	std::vector<Block*> mBlock;

	//見えない壁(当たり判定用)
	std::vector<Model*> mInvisibleAABBModel; //見えない壁のvector
	std::vector<Transform> mInvisibleAABBTransform;
	std::vector<AABB> mInvisibleAABB; 
};

