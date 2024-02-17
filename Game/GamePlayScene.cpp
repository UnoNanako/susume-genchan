#include "DirectXCommon.h"
#include "GamePlayScene.h"
#include "3D/Model.h"
#include "3D/SphereModel.h"
#include "2D/Sprite.h"
#include "2D/Texture.h"
#include "Transform.h"
#include "VertexData.h"
#include "Camera.h"
#include "Light/LightList.h"
#include "Player.h"
#include "Map.h"
#include "MyMath.h"
#include "Crosshair.h"
#include "Particle/ParticleList.h"
#include "Game/Game.h"
#include "3D/ModelCommon.h"
#include "Particle/ParticleCommon.h"

GamePlayScene::GamePlayScene(Game* game)
{
	mGame = game;
}

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//カメラの初期化
	camera = new Camera;
	camera->Initialize(dxCommon);

	lightList = new LightList();
	lightList->Create(dxCommon);

	//player
	mPlayer = new Player();
	mPlayer->Initialize(dxCommon);
	mPlayer->SetLightList(lightList);

	//Map
	mMap = new Map();
	mMap->Create(dxCommon);

	//crosshair
	mCrosshair = new Crosshair();
	mCrosshair->Initialize(dxCommon);

	//particle
	mParticle = new ParticleList();
	mParticle->Create(dxCommon);
}

void GamePlayScene::Finalize()
{
	delete camera;
	delete mPlayer;
	delete mMap;
	delete lightList;
	delete mCrosshair;
	delete mParticle;
}

void GamePlayScene::Update(Input* input)
{
	Transform spriteTransform = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	camera->Update();
	lightList->Update();
	mPlayer->Update(input);
	mMap->Update();
	mCrosshair->Update();
	mParticle->Update(camera);
	CollisionResult collisionResult;

	//見えない壁との当たり判定
	/*for (int i = 0; i < 4; i++) {
		if (IsCollision(mPlayer->GetAABB(), mMap->GetAABBInvisible()[i], collisionResult)) {
			mPlayer->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth;
			pos.y += collisionResult.normal.y * collisionResult.depth;
			pos.z += collisionResult.normal.z * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}*/
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	mGame->GetModelCommon()->Bind(dxCommon);
	camera->Bind(dxCommon->GetCommandList());
	lightList->Bind(dxCommon->GetCommandList());
	mPlayer->Draw(dxCommon->GetCommandList(),camera);
	mMap->Draw(dxCommon->GetCommandList(), camera);
	mCrosshair->Draw(dxCommon->GetCommandList());

	mGame->GetParticleCommon()->Bind(dxCommon);
	//mParticle->Draw(dxCommon->GetCommandList(), camera, { 0.0f,0.0f,0.0f });
}
