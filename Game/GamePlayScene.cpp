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
#include "Engine/Input/Input.h"
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
#include "Block.h"
#include "Engine//3D/Model.h"

GamePlayScene::GamePlayScene(Game* game){
	mGame = game;
}

void GamePlayScene::Initialize(DirectXCommon* dxCommon){
	//カメラの初期化
	mPlayerCamera = std::make_unique<PlayerCamera>();
	mPlayerCamera->Initialize(dxCommon);
	//俯瞰カメラ
	mBirdEyeCamera = std::make_unique<BirdEyeCamera>();
	mBirdEyeCamera->Initialize(dxCommon);
	//ライト
	mLightList = std::make_unique<LightList>();
	mLightList->Create(dxCommon);
	//パーティクル
	mParticle = std::make_unique<ParticleList>();
	mParticle->Create(dxCommon);
	//タイトルシーン
	mTitleScene = std::make_unique<TitleScene>();

	/// <summary>
	/// プレイヤー
	/// <summary>
	mPlayer = std::make_unique<Player>();
	mPlayer->Initialize(dxCommon);
	mPlayer->SetLightList(mLightList.get());

	/// <summary>
	/// オブジェクト
	/// <summary>
	//地形
	mMap = std::make_unique<Map>();
	mMap->Create(dxCommon);
	//天球
	mSkydome = std::make_unique<Skydome>();
	mSkydome->Initialize(dxCommon);
	//草
	mGrasses.resize(mGRASS_MAX);
	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		mGrasses[i] = std::make_unique<Grass>();
		mGrasses[i]->Initialize(dxCommon);
	}
	mGrasses[0]->SetTranslate({ -12.5f,2.0f,20.0f });
	//ジェム
	mGems.resize(1);
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i] = std::make_unique<Gem>();
		mGems[i]->Initialize(dxCommon);
	}
	mGems[0]->SetTranslate({ 22.0f,5.0f,-22.5f });
	//スター
	mStar = std::make_unique<Star>();
	mStar->Initialize(dxCommon);
	mStar->SetTranslate({ -5.0f,30.0f,85.0f });
	//movingFloor(スイッチを押すとスライドし始める床)
	mSlideFloor = std::make_unique<SlideFloor>();
	mSlideFloor->Initialize(dxCommon);
	//スイッチ
	mSlideSwitch = std::make_unique<Switch>();
	mSlideSwitch->SetMoveFloor(mSlideFloor.get());
	mSlideSwitch->Initialize(dxCommon);
	mSlideSwitch->SetTransform({ {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{27.5f,2.5f,-20.0f} });
	//クランクモデル
	mCrank = std::make_unique<Crank>();
	mCrank->Initialize(dxCommon);
	//クランクを回すと回る床
	mRotateFloor = std::make_unique<RotateFloor>();
	mRotateFloor->Initialize(dxCommon);
	
	LadderInitialize(dxCommon); //はしごの初期化

	/// <summary>
	/// 敵
	/// <summary>
	//rotateEnemy(回転するだけの敵)
	mRotateEnemies.resize(mROTATEENEMY_MAX);
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		mRotateEnemies[i] = std::make_unique<RotateEnemy>();
		mRotateEnemies[i]->Initialize(dxCommon);
	}
	mRotateEnemies[0]->SetTranslate({ 15.0f,2.0f,12.5f });
	//walkEnemy(歩き回る敵)
	mWalkEnemies.resize(mWALKENEMY_MAX);
	for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
		mWalkEnemies[i] = std::make_unique<WalkEnemy>();
		mWalkEnemies[i]->Initialize(dxCommon);
	}
	mWalkEnemies[0]->SetTranslate({ -7.5f,2.0f,25.0f });
	//ghost(テレサ)
	mGhosts.resize(mGHOST_MAX);
	for (uint32_t i = 0; i < mGhosts.size(); ++i) {
		mGhosts[i] = std::make_unique<Ghost>();
		mGhosts[i]->Initialize(dxCommon);
	}
	mGhosts[0]->SetTranslate({ 22.5f,5.0f,-5.0f });
	mGhosts[0]->SetRotate({ 0.0f,-kPi / 2.0f,0.0f });

	/// <summary>
	/// スプライト
	/// <summary>
	//クロスヘア
	mCrosshair = std::make_unique<Crosshair>();
	mCrosshair->Initialize(dxCommon);
	//クリアテキスト
	mClearSprite = std::make_unique<Sprite>();
	mClearSprite->Create(dxCommon, "resources/Sprite/Text/COURSE_CLEAR.png");
	mClearSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{100.0f,300.0f,0.0f} });
	//ゲームオーバーテキスト
	mGameoverSprite = std::make_unique<Sprite>();
	mGameoverSprite->Create(dxCommon, "resources/Sprite/Text/GAME_OVER.png");
	mGameoverSprite->SetTransform({ {1.0f,1.0f,1.0f,},{0.0f,0.0f,0.0f},{200.0f,300.0f,0.0f} });

	/// <summary>
	/// 当たり判定の壁
	/// <summary>
	mCollisionWalls.resize(mCOLLISIONWALL_MAX);
	for (uint32_t i = 0; i < mCOLLISIONWALL_MAX; ++i) {
		mCollisionWalls[i] = std::make_unique<CollisionWall>();
		mCollisionWalls[i]->Initialize();
	}
	//外周（左から時計回り）
	mCollisionWalls[0]->SetTransform({{3.0f,5.0f,50.0f},{0.0f,0.0f,0.0f},{-30.0f,5.0f,0.0f}});
	mCollisionWalls[1]->SetTransform({ {30.0f,5.0f,3.0f},{0.0f,0.0f,0.0f},{-15.0f,5.0f,27.5f}});
	mCollisionWalls[2]->SetTransform({ {3.0f,5.0f,50.0f},{0.0f,0.0f,0.0f},{30.0f,5.0f,0.0f} });
	mCollisionWalls[3]->SetTransform({ {60.0f,5.0f,3.0f},{0.0f,0.0f,0.0f},{0.0f,5.0f,-27.5f} });
	//草
	mCollisionWalls[4]->SetTransform({ {5.5f,5.5f,40.0f},{0.0f,0.0f,0.0f},{-17.5f,5.0f,-7.5f} });
	mCollisionWalls[5]->SetTransform({ {5.5f,5.0f,15.0f},{0.0f,0.0f,0.0f},{-22.5f,5.0f,15.0f} });
	mCollisionWalls[6]->SetTransform({ {15.5f,5.0f,10.5f},{0.0f,0.0f,0.0f},{-2.5f,5.0f,-17.5f} });
	mCollisionWalls[7]->SetTransform({ {5.5f,5.0f,10.5f},{0.0f,0.0f,0.0f},{-7.5f,5.0f,-7.5f} });
}

void GamePlayScene::Finalize()
{
	mMap->Finalize();
}

void GamePlayScene::Update(Input* input){
	/// <summary>
	/// ImGui
	/// <summary>
	ImGui::Begin("Debug");
	//DirectionalLightのintensity切り替え
	ImGui::Checkbox("isDirectionalLight", &mIsDirectionalLight);
	ImGui::Checkbox("isPlayerCamera", &mIsPlayerCamera);
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		Vector3 pos = mLadders[i]->GetTranslate();
		ImGui::DragFloat3("ladderPos", &pos.x);
		mLadders[i]->SetTranslate(pos);
	}
	ImGui::End();

	if (mIsDirectionalLight == false) {
		mLightList->SetDirectionalLightIntensity(0.0f);
	}
	else {
		mLightList->SetDirectionalLightIntensity(0.7f);
	}

	ObjectUpdate(input); //オブジェクトの更新
	
	Collision(input); //当たり判定の更新

	//プレイヤーの行列を更新
	mPlayer->GetTransform().UpdateMatrix();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon){
	mGame->GetModelCommon()->Bind(dxCommon);
	mLightList->Bind(dxCommon->GetCommandList());
	//----------モデルここから----------
	if (mIsPlayerCamera == true) {
		mPlayerCamera->Bind(dxCommon->GetCommandList());
		mPlayerCamera->SetTransform(mPlayer->GetTransform());
		mPlayer->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		mMap->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
			mRotateEnemies[i]->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		}
		for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
			mWalkEnemies[i]->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		}
		for (uint32_t i = 0; i < mGhosts.size(); ++i) {
			mGhosts[i]->Draw(dxCommon->GetCommandList(), mPlayerCamera.get());
		}
	}
	else {
		mBirdEyeCamera->Bind(dxCommon->GetCommandList());
		mPlayer->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		mMap->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
			mRotateEnemies[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		}
		for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
			mWalkEnemies[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		}
		for (uint32_t i = 0; i < mGhosts.size(); ++i) {
			mGhosts[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
		}
	}

	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}

	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		mGrasses[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}

	mStar->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mSlideFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mSlideSwitch->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mCrank->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mRotateFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mSkydome->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());

	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		mLadders[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	//----------モデルここまで----------

	mGame->GetParticleCommon()->Bind(dxCommon);
	//mParticle->Draw(dxCommon->GetCommandList(), camera, { 0.0f,0.0f,0.0f });
	mGame->GetModelCommon()->Bind(dxCommon);
	mCrosshair->Draw(dxCommon->GetCommandList());

	if (mCrank->GetIsHit() == true) {
		//mCrankSprite->Draw(dxCommon->GetCommandList());
	}
	if (mIsClear == true) {
		mClearSprite->Draw(dxCommon->GetCommandList());
	}
	if (mIsGameover == true) {
		//mGameoverSprite->Draw(dxCommon->GetCommandList());
	}
}

void GamePlayScene::LadderInitialize(DirectXCommon* dxCommon){
	//はしご
	mLadders.resize(mLADDER_MAX);
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		mLadders[i] = std::make_unique<Ladder>();
		mLadders[i]->Initialize(dxCommon);
	}
	//モデルの作成
	mLadderModel_height15 = std::make_unique<Model>();
	mLadderModel_height15->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	mLadderModel_height15_02 = std::make_unique<Model>();
	mLadderModel_height15_02->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	//離島にあるはしご
	mLadders[0]->SetScale({ 0.5f,0.5f,0.5f });
	mLadders[0]->SetTranslate({ -3.0f,13.0f,87.5f });
	mLadders[0]->SetHeight(30.0f);
	mLadders[0]->SetDirection(Ladder::RIGHT); //右向き
	//リス地に一番近いはしご
	mLadders[1]->SetModel(mLadderModel_height15.get());
	mLadders[1]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[1]->SetTranslate({ -2.5f,10.0f,-22.5f });
	mLadders[1]->SetRotate({ 0.0f,-kPi / 2.0f,0.0f });
	mLadders[1]->SetHeight(15.0f);
	mLadders[1]->SetDirection(Ladder::FRONT); //手前向き
	//クランクを回すためのはしご
	mLadders[2]->SetModel(mLadderModel_height15_02.get());
	mLadders[2]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[2]->SetTranslate({ 25.0f,10.0f,0.0f });
	mLadders[2]->SetHeight(15.0f);
	mLadders[2]->SetDirection(Ladder::RIGHT); //右向き
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		switch (mLadders[i]->GetDirection()) {
		case Ladder::FRONT:
			mLadders[i]->SetVec({ 0.0f,0.0f,1.0f });
			break;
		case Ladder::BACK:
			mLadders[i]->SetVec({ 0.0f,0.0f,-1.0f });
			break;
		case Ladder::LEFT:
			mLadders[i]->SetVec({ 1.0f,0.0f,0.0f });
			break;
		case Ladder::RIGHT:
			mLadders[i]->SetVec({ -1.0f,0.0f,0.0f });
			break;
		}
	}
}

void GamePlayScene::ObjectUpdate(Input *input){
	/// <summary>
	/// 更新
	/// <summary>
	mPlayer->Update(input, mBirdEyeCamera->GetLon()); //プレイヤー
	mLightList->Update(); //ライト
	//プレイヤーカメラ
	if (mIsPlayerCamera == true) {
		mPlayerCamera->Update();
	}
	else {
		mBirdEyeCamera->Update(input, mPlayer->GetWorldPosition());
	}
	mMap->Update(); //マップ
	mSkydome->Update(); //天球
	mParticle->Update(mPlayerCamera.get()); //パーティクル
	//回転する敵
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		mRotateEnemies[i]->Update();
		mRotateEnemies[i]->TrackPlayer(mPlayer.get());
	}
	//歩く敵
	for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
		mWalkEnemies[i]->Update();
		mGhosts[i]->TrackPlayer(mPlayer.get());
	}
	//ゴースト(テレサ)
	for (uint32_t i = 0; i < mGhosts.size(); ++i) {
		mGhosts[i]->Update();
	}
	//ジェム
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Update();
	}
	//草オブジェクト
	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		mGrasses[i]->Update();
	}
	mStar->Update();
	mSlideFloor->Update();
	mSlideSwitch->Update();
	mCrank->Update(input);
	mRotateFloor->Update();
	mRotateFloor->SetRotate(mCrank->GetRotate());
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		mLadders[i]->Update();
	}
	for (uint32_t i = 0; i < mCOLLISIONWALL_MAX; ++i) {
		mCollisionWalls[i]->Update();
	}
	mClearSprite->Update(); //クリアスプライト
	mGameoverSprite->Update(); //ゲームオーバースプライト
	mCrosshair->Update(); //クロスヘア
}

void GamePlayScene::Collision(Input *input){
	CollisionResult collisionResult;
	//壁,床とプレイヤー
	mPlayer->SetIsHit(false);
	for (uint32_t i = 0; i < mMap->GetBlock().size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mMap->GetBlock()[i]->GetWorldAABB(), collisionResult)) {
			mPlayer->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos += collisionResult.normal * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//当たり判定用の壁とプレイヤー
	for (uint32_t i = 0; i < mCOLLISIONWALL_MAX; ++i) {
		if (IsCollision(mPlayer->GetAABB(), mCollisionWalls[i]->GetAABB(), collisionResult)) {
			ImGui::Begin("Debug");
			ImGui::Text("Hit");
			ImGui::End();
			Vector3 pos = mPlayer->GetTranslate();
			pos += collisionResult.normal * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//rotateEnemyとプレイヤー
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mRotateEnemies[i]->GetAABB(), collisionResult)) {
			mIsGameover = true;
		}
	}

	//壁とrotateEnemy
	for (uint32_t i = 0; i < mMap->GetBlock().size(); ++i) {
		for (uint32_t j = 0; j < mRotateEnemies.size(); ++j) {
			if (IsCollision(mRotateEnemies[j]->GetAABB(), mMap->GetBlock()[i]->GetWorldAABB(), collisionResult)) {
				Vector3 pos = mRotateEnemies[j]->GetTranslate();
				pos.x += collisionResult.normal.x * collisionResult.depth;
				pos.z += collisionResult.normal.z * collisionResult.depth;
				mRotateEnemies[j]->SetTranslate(pos);
			}
		}
	}

	//ghostとプレイヤー
	for (uint32_t i = 0; i < mGhosts.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mGhosts[i]->GetAABB(), collisionResult)) {
			mIsGameover = true;
		}
	}

	//starとプレイヤー
	if (IsCollision(mPlayer->GetAABB(), mStar->GetAABB(), collisionResult)) {
		mIsClear = true;
	}

	//gemとプレイヤー
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mGems[i]->GetAABB(), collisionResult)) {
			mGems[i]->SetIsHit(true);
		} else {
			mGems[i]->SetIsHit(false);
		}
	}

	//grassとプレイヤー
	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mGrasses[i]->GetAABB(), collisionResult)) {
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth;
			pos.z += collisionResult.normal.z * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//SlideSwitchとプレイヤー
	if (IsCollision(mPlayer->GetAABB(), mSlideSwitch->GetAABB(), collisionResult)) {
		mSwitchIsHit = true;
		mSlideFloor->SetIsMoving(true);
	}else {
		mSwitchIsHit = false;
	}

	//slideFloorとプレイヤー
	if (IsCollision(mPlayer->GetAABB(), mSlideFloor->GetAABB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		mSlideFloorIsHit = true;
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth;
		pos.y += collisionResult.normal.y * collisionResult.depth;
		pos.z += collisionResult.normal.z * collisionResult.depth;
		mPlayer->SetTranslate(pos);
		if (mPlayer->GetParent() == nullptr) {
			//プレイヤーとslideFloorの親子関係を結ぶ
			Matrix4x4 local = Multiply(mPlayer->GetWorldMatrix(), Inverse(mSlideFloor->GetWorldMatrix()));
			mPlayer->SetTranslate(Vector3{ local.m[3][0],local.m[3][1],local.m[3][2] });
		}
		auto& tmp = mSlideFloor->GetTransform();
		mPlayer->SetParent(&tmp);
	}else {
		if (mSlideFloorIsHit == true) {
			mPlayer->SetParent(nullptr);
			mSlideFloorIsHit = false;
			Matrix4x4 world = mPlayer->GetWorldMatrix();
			mPlayer->SetTranslate(Vector3{ world.m[3][0],world.m[3][1],world.m[3][2] });
		}
	}

	//クランクとプレイヤー
	if (IsCollision(mPlayer->GetAABB(), mCrank->GetAABB(), collisionResult)) {
		mCrank->SetIsHit(true);
		mBirdEyeCamera->SetIsHit(true);
	}else {
		mCrank->SetIsHit(false);
		mBirdEyeCamera->SetIsHit(false);
	}

	//回転する床とプレイヤー
	if (IsCollision(mPlayer->GetAABB(), mRotateFloor->GetOBB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth / 2;
		pos.y += collisionResult.normal.y * collisionResult.depth / 2;
		pos.z += collisionResult.normal.z * collisionResult.depth / 2;
		mPlayer->SetTranslate(pos);
	}

	//プレイヤーとはしご
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mLadders[i]->GetAABB(), collisionResult)) {
			mLadders[i]->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth / 2;
			pos.y += collisionResult.normal.y * collisionResult.depth / 2;
			pos.z += collisionResult.normal.z * collisionResult.depth / 2;
			mPlayer->SetTranslate(pos);
		}else {
			mLadders[i]->SetIsHit(false);
		}
	}
	bool isHitLadder = false;

	//プレイヤーとはしごが当たっているとき
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		if (mLadders[i]->GetIsHit() == true) {
			isHitLadder = true;
			//はしごの向き(ベクトル)
			Vector3 ladderVec = mLadders[i]->GetVec();
			//プレイヤーの向き
			Vector3 forwardVec = Multiply(Vector3(0.0f, 0.0f, 1.0f), MakeRotateYMatrix(mPlayer->GetRotate().y));
			//内積を計算
			float dotProduct = Dot(forwardVec, ladderVec);
			if (dotProduct >= 0.9f || mPlayer->GetTranslate().y < mLadders[i]->GetHeight() - 5.0f)
			{
				if (input->PushKey(DIK_W) || input->GetLStick().y >= 0.7f) {
					switch (mLadders[i]->GetDirection()) {
					case Ladder::FRONT:
						mPlayer->SetTranslate(
							{ mLadders[i]->GetTranslate().x,
							mPlayer->GetTranslate().y,
							mLadders[i]->GetTranslate().z - 1.5f }
						);
						if (mPlayer->GetTranslate().y >= 17.5f) {
							mPlayer->SetTranslate({ mPlayer->GetTranslate().x,mPlayer->GetTranslate().y + 2.0f,mPlayer->GetTranslate().z + 2.0f });
						}
						break;
					case Ladder::BACK:
						mPlayer->SetTranslate(
							{ mLadders[i]->GetTranslate().x,
							mPlayer->GetTranslate().y,
							mLadders[i]->GetTranslate().z + 1.5f }
						);
						if (mPlayer->GetTranslate().y >= 17.5) {
							mPlayer->SetTranslate({ mPlayer->GetTranslate().x,mPlayer->GetTranslate().y + 2.0f,mPlayer->GetTranslate().z - 2.0f });
						}
						break;
					case Ladder::LEFT:
						mPlayer->SetTranslate(
							{ mLadders[i]->GetTranslate().x - 1.5f,
							mPlayer->GetTranslate().y,
							mLadders[i]->GetTranslate().z }
						);
						if (mPlayer->GetTranslate().y >= 17.5f) {
							mPlayer->SetTranslate({ mPlayer->GetTranslate().x + 2.0f, mPlayer->GetTranslate().y + 2.0f,mPlayer->GetTranslate().z });
						}
						break;
					case Ladder::RIGHT:
						mPlayer->SetTranslate(
							{ mLadders[i]->GetTranslate().x + 1.5f,
							mPlayer->GetTranslate().y,
							mLadders[i]->GetTranslate().z }
						);
						if (mPlayer->GetTranslate().y >= 17.5f) {
							mPlayer->SetTranslate({ mPlayer->GetTranslate().x - 2.0f, mPlayer->GetTranslate().y + 2.0f,mPlayer->GetTranslate().z });
						}
						break;
					}

					Vector3 pos = mPlayer->GetTranslate();
					pos.y += 0.2f;
					mPlayer->SetTranslate(pos);
				}
			}
		}
	}
	if (isHitLadder == true) {
		mPlayer->SetGravity(0.0f);
	}else {
		mPlayer->SetGravity(0.05f);
	}
}
