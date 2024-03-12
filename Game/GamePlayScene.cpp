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
#include "Game/MoveFloor.h"
#include "Game/SlideFloor.h"
#include "Game/UpFloor.h"
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
	mPlayerCamera = std::make_unique<PlayerCamera>();
	mPlayerCamera->Initialize(dxCommon);

	//俯瞰カメラ
	mBirdEyeCamera = std::make_unique<BirdEyeCamera>();
	mBirdEyeCamera->Initialize(dxCommon);

	//ライト
	mLightList = std::make_unique<LightList>();
	mLightList->Create(dxCommon);

	//player
	mPlayer = std::make_unique<Player>();
	mPlayer->Initialize(dxCommon);
	mPlayer->SetLightList(mLightList.get());

	//Map
	mMap = std::make_unique<Map>();
	mMap->Create(dxCommon);

	//crosshair
	mCrosshair = std::make_unique<Crosshair>();
	mCrosshair->Initialize(dxCommon);

	//particle
	mParticle = std::make_unique<ParticleList>();
	mParticle->Create(dxCommon);

	//rotateEnemy
	mRotateEnemies.resize(mROTATEENEMY_MAX);
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		mRotateEnemies[i] = std::make_unique<RotateEnemy>();
		mRotateEnemies[i]->Initialize(dxCommon);
	}
	mRotateEnemies[0]->SetTranslate({ 15.0f,2.0f,12.5f });

	//gem
	mGems.resize(1);
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i] = std::make_unique<Gem>();
		mGems[i]->Initialize(dxCommon);
	}
	mGems[0]->SetTranslate({ 17.5f,10.0f,-15.0f });

	//star
	mStar = std::make_unique<Star>();
	mStar->Initialize(dxCommon);
	mStar->SetTranslate({17.5f,5.0f,80.0f});

	//movingFloor
	mSlideFloor = std::make_unique<SlideFloor>();
	mSlideFloor->Initialize(dxCommon);
	//switch
	mSlideSwitch = std::make_unique<Switch>();
	mSlideSwitch->SetMoveFloor(mSlideFloor.get());
	mSlideSwitch->Initialize(dxCommon);

	//UpDownFloor(スイッチを押すと上下に動く床)
	//mUpFloor = std::make_unique<UpFloor>();
	//mUpFloor->Initialize(dxCommon);
	//upSwitch
	mUpSwitch = std::make_unique<Switch>();
	mUpSwitchModel = std::make_unique<Model>();
}

void GamePlayScene::Finalize()
{
	mMap->Finalize();
}

void GamePlayScene::Update(Input* input)
{
	ImGui::Begin("Debug");
	//DirectionalLightのintensity切り替え
	ImGui::Checkbox("isDirectionalLight", &mIsDirectionalLight);
	ImGui::Checkbox("isPlayerCamera", &mIsPlayerCamera);
	ImGui::End();

	if (mIsDirectionalLight == false) {
		mLightList->SetDirectionalLightIntensity(0.0f);
	}
	else {
		mLightList->SetDirectionalLightIntensity(0.7f);
	}
	Transform spriteTransform = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	mLightList->Update();
	mSlideFloor->Update();
	mPlayer->Update(input,mBirdEyeCamera->GetLon());

	if (mIsPlayerCamera == true) {
		mPlayerCamera->Update();
	}
	else {
		mBirdEyeCamera->Update(input,mPlayer->GetWorldPosition());
	}

	mMap->Update();
	mCrosshair->Update();
	mParticle->Update(mPlayerCamera.get());
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		mRotateEnemies[i]->Update();
		mRotateEnemies[i]->DetectPlayer(mPlayer.get());
		mRotateEnemies[i]->TrackPlayer(mPlayer.get());
	}
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Update();
	}
	mStar->Update();
	mSlideSwitch->Update();
	//mUpFloor->Update();

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
	if (IsCollision(mPlayer->GetAABB(), mSlideSwitch->GetAABB(), collisionResult)) {
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth;
		pos.y += collisionResult.normal.y * collisionResult.depth;
		pos.z += collisionResult.normal.z * collisionResult.depth;
		mPlayer->SetTranslate(pos);
		mSlideFloor->SetIsMoving(true);
	}

	//movingFloorとplayerの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mSlideFloor->GetAABB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		mIsHit = true;
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth;
		pos.y += collisionResult.normal.y * collisionResult.depth;
		pos.z += collisionResult.normal.z * collisionResult.depth;
		mPlayer->SetTranslate(pos);
		if (mPlayer->GetParent() == nullptr) {
			//playerとmoveFloorの親子関係を結ぶ
			Matrix4x4 local = Multiply(mPlayer->GetWorldMatrix(), Inverse(mSlideFloor->GetWorldMatrix()));
			mPlayer->SetTranslate(Vector3{ local.m[3][0],local.m[3][1],local.m[3][2] });
		}
		auto& tmp = mSlideFloor->GetTransform();
		mPlayer->SetParent(&tmp);
	}
	else {
		mPlayer->SetParent(nullptr);
		if (mIsHit == true) {
			mIsHit = false;
			Matrix4x4 world = mPlayer->GetWorldMatrix();
			mPlayer->SetTranslate(Vector3{ world.m[3][0],world.m[3][1],world.m[3][2] });
		}
	}
	mPlayer->GetTransform().UpdateMatrix();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	mGame->GetModelCommon()->Bind(dxCommon);
	mLightList->Bind(dxCommon->GetCommandList());
	if (mIsPlayerCamera == true) {
		mPlayerCamera->Bind(dxCommon->GetCommandList());
		mPlayerCamera->SetTransform(mPlayer->GetTransform());
		mPlayer->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		mMap->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
			mRotateEnemies[i]->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		}
	}
	else {
		mBirdEyeCamera->Bind(dxCommon->GetCommandList());
		mPlayer->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		mMap->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
			mRotateEnemies[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		}
	}
	
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	mStar->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mSlideFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mSlideSwitch->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	//mUpFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mGame->GetParticleCommon()->Bind(dxCommon);
	//mParticle->Draw(dxCommon->GetCommandList(), camera, { 0.0f,0.0f,0.0f });
	mGame->GetModelCommon()->Bind(dxCommon);
	mCrosshair->Draw(dxCommon->GetCommandList());
}
