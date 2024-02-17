#pragma once
#include <d3d12.h>
#include "Transform.h"
#include "GameObject.h"
#include <vector>

using namespace std;

class Texture;
class Model;
struct AABB;

class Map : public GameObject
{
public:
	void Create(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);
	
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
	vector<Model*> mTerrainModel; //壁や床など
	vector<Transform> mTerrainTransform;
};

