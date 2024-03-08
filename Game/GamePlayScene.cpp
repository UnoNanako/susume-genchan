#include "DirectXCommon.h"
#include "GamePlayScene.h"
#include "3D/Model.h"
#include "3D/SphereModel.h"
#include "2D/Sprite.h"
#include "2D/Texture.h"
#include "Transform.h"
#include "VertexData.h"
#include "PlayerCamera.h"
#include "BirdEyeCamera.h"
#include "Light/LightList.h"
#include "Player.h"
#include "Map.h"
#include "Game/Switch.h"
#include "MyMath.h"
#include "Gem.h"
#include "Game/Star.h"
#include "Game/MovingFloor.h"
#include "Crosshair.h"
#include "Particle/ParticleList.h"
#include "Game/Game.h"
#include "3D/ModelCommon.h"
#include "Game/RotateEnemy.h"
#include "Particle/ParticleCommon.h"
#include "externals/imgui/imgui.h"

GamePlayScene::GamePlayScene(Game* game)
{
	mGame = game;
}

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//カメラの初期化
	mPlayerCamera = new PlayerCamera;
	mPlayerCamera->Initialize(dxCommon);

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

	//rotateEnemy
	mRotateEnemies.resize(mROTATEENEMY_MAX);
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		mRotateEnemies[i] = new RotateEnemy();
		mRotateEnemies[i]->Initialize(dxCommon);
	}
	mRotateEnemies[0]->SetTranslate({ 15.0f,2.0f,12.5f });

	//gem
	mGems.resize(1);
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i] = new Gem();
		mGems[i]->Initialize(dxCommon);
	}
	mGems[0]->SetTranslate({ 17.5f,10.0f,-15.0f });

	//star
	mStar = new Star();
	mStar->Initialize(dxCommon);
	mStar->SetTranslate({17.5f,5.0f,80.0f});

	//movingFloor
	mMovingFloor = new MovingFloor();
	mMovingFloor->Initialize(dxCommon);

	//switch
	mSwitch = new Switch();
	mSwitch->Initialize(dxCommon);
}

void GamePlayScene::Finalize()
{
	mMap->Finalize();
	delete mPlayerCamera;
	delete mBirdEyeCamera;
	delete mPlayer;
	delete mMap;
	delete lightList;
	delete mCrosshair;
	delete mParticle;
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		delete mRotateEnemies[i];
	}
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		delete mGems[i];
	}
	delete mStar;
	delete mMovingFloor;
	delete mSwitch;
}

void GamePlayScene::Update(Input* input)
{
	ImGui::Begin("Debug");
	//DirectionalLightのintensity切り替え
	ImGui::Checkbox("isDirectionalLight", &mIsDirectionalLight);
	ImGui::Checkbox("isPlayerCamera", &mIsPlayerCamera);
	ImGui::End();

	if (mIsDirectionalLight == false) {
		lightList->SetDirectionalLightIntensity(0.0f);
	}
	else {
		lightList->SetDirectionalLightIntensity(0.7f);
	}
	Transform spriteTransform = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	lightList->Update();
	mPlayer->Update(input,mBirdEyeCamera->GetLon());

	if (mIsPlayerCamera == true) {
		mPlayerCamera->Update();
	}
	else {
		mBirdEyeCamera->Update(input,mPlayer->GetTranslate());
	}

	mMap->Update();
	mCrosshair->Update();
	mParticle->Update(mPlayerCamera);
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		mRotateEnemies[i]->Update();
		mRotateEnemies[i]->DetectPlayer(mPlayer);
		mRotateEnemies[i]->TrackPlayer(mPlayer);
	}
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Update();
	}
	mStar->Update();
	mMovingFloor->Update();
	mSwitch->Update();

	CollisionResult collisionResult;
	//壁とプレイヤーの当たり判定
	mPlayer->SetIsHit(false);
	for (uint32_t i = 0; i < mMap->GetTerrainModel().size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mMap->GetTerrainAABB()[i], collisionResult)) {
			mPlayer->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth;
			pos.y += collisionResult.normal.y * collisionResult.depth;
			pos.z += collisionResult.normal.z * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//rotateEnemyとプレイヤーの当たり判定
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mRotateEnemies[i]->GetAABB(), collisionResult)) {
			ImGui::Begin("Debug");
			ImGui::Text("Dead");
			ImGui::End();
		}
	}

	//壁とrotateEnemyの当たり判定
	for (uint32_t i = 0; i < mMap->GetTerrainModel().size(); ++i) {
		for (uint32_t j = 0; j < mRotateEnemies.size(); ++j) {
			if (IsCollision(mRotateEnemies[j]->GetAABB(), mMap->GetTerrainAABB()[i], collisionResult)) {
				Vector3 pos = mRotateEnemies[j]->GetTranslate();
				pos.x += collisionResult.normal.x * collisionResult.depth;
				pos.z += collisionResult.normal.z * collisionResult.depth;
				mRotateEnemies[j]->SetTranslate(pos);
			}
		}
	}

	//starとplayerの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mStar->GetAABB(), collisionResult)) {
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth;
		pos.z += collisionResult.normal.z * collisionResult.depth;
		mPlayer->SetTranslate(pos);
	}

	//switchとplayerの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mSwitch->GetAABB(), collisionResult)) {
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth;
		pos.y += collisionResult.normal.y * collisionResult.depth;
		pos.z += collisionResult.normal.z * collisionResult.depth;
	}
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	mGame->GetModelCommon()->Bind(dxCommon);
	lightList->Bind(dxCommon->GetCommandList());
	if (mIsPlayerCamera == true) {
		mPlayerCamera->Bind(dxCommon->GetCommandList());
		mPlayerCamera->SetTransform(mPlayer->GetTransform());
		mPlayer->Draw(dxCommon->GetCommandList(), mPlayerCamera);
		mMap->Draw(dxCommon->GetCommandList(), mPlayerCamera);
		for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
			mRotateEnemies[i]->Draw(dxCommon->GetCommandList(), mPlayerCamera);
		}
	}
	else {
		mBirdEyeCamera->Bind(dxCommon->GetCommandList());
		mPlayer->Draw(dxCommon->GetCommandList(), mBirdEyeCamera);
		mMap->Draw(dxCommon->GetCommandList(), mBirdEyeCamera);
		for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
			mRotateEnemies[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera);
		}
	}
	
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera);
	}
	mStar->Draw(dxCommon->GetCommandList(), mBirdEyeCamera);
	mMovingFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera);
	mSwitch->Draw(dxCommon->GetCommandList(), mBirdEyeCamera);
	mGame->GetParticleCommon()->Bind(dxCommon);
	//mParticle->Draw(dxCommon->GetCommandList(), camera, { 0.0f,0.0f,0.0f });

	mGame->GetModelCommon()->Bind(dxCommon);
	mCrosshair->Draw(dxCommon->GetCommandList());
}
