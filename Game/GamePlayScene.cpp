#include "DirectXCommon.h"
#include "GamePlayScene.h"
#include "3D/Model.h"
#include "3D/SphereModel.h"
#include "2D/Sprite.h"
#include "2D/Texture.h"
#include "Transform.h"
#include "VertexData.h"
#include "Camera.h"
#include "BirdEyeCamera.h"
#include "Light/LightList.h"
#include "Player.h"
#include "Map.h"
#include "MyMath.h"
#include "Crosshair.h"
#include "Particle/ParticleList.h"
#include "Game/Game.h"
#include "3D/ModelCommon.h"
#include "Particle/ParticleCommon.h"
#include "externals/imgui/imgui.h"

GamePlayScene::GamePlayScene(Game* game)
{
	mGame = game;
}

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//カメラの初期化
	camera = new Camera;
	camera->Initialize(dxCommon);

	mBirdEyeCamera = new BirdEyeCamera();
	mBirdEyeCamera->Initialize(dxCommon);

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
	delete mBirdEyeCamera;
	delete mPlayer;
	delete mMap;
	delete lightList;
	delete mCrosshair;
	delete mParticle;
}

void GamePlayScene::Update(Input* input)
{
	ImGui::Begin("Debug");
	//DirectionalLightのintensity切り替え
	ImGui::Checkbox("isDirectionalLight", &mIsDirectionalLight);
	//ImGui::Checkbox("isPlayerCamera", &mIsPlayerCamera);
	ImGui::End();

	if (mIsDirectionalLight == false) {
		lightList->SetDirectionalLightIntensity(0.0f);
	}
	else {
		lightList->SetDirectionalLightIntensity(0.7f);
	}

	if (mIsPlayerCamera == true) {
		camera->Update();
	}
	else {
		mBirdEyeCamera->Update();
	}

	Transform spriteTransform = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	lightList->Update();
	mPlayer->Update(input);
	mMap->Update();
	mCrosshair->Update();
	mParticle->Update(camera);
	CollisionResult collisionResult;

	//壁との当たり判定
	for (uint32_t i = 0; i < mMap->GetTerrainModel().size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mMap->GetTerrainAABB()[i], collisionResult)) {
			mPlayer->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth;
			//pos.y += collisionResult.normal.y * collisionResult.depth;
			pos.z += collisionResult.normal.z * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

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
	if (mIsPlayerCamera == true) {
		camera->Bind(dxCommon->GetCommandList());
	}
	else {
		mBirdEyeCamera->Bind(dxCommon->GetCommandList());
	}

	lightList->Bind(dxCommon->GetCommandList());
	mPlayer->Draw(dxCommon->GetCommandList(),camera);
	mMap->Draw(dxCommon->GetCommandList(), camera);
	mCrosshair->Draw(dxCommon->GetCommandList());

	mGame->GetParticleCommon()->Bind(dxCommon);
	//mParticle->Draw(dxCommon->GetCommandList(), camera, { 0.0f,0.0f,0.0f });
}
