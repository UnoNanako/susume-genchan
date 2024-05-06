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

	//プレイヤー
	mPlayer = std::make_unique<Player>();
	mPlayer->Initialize(dxCommon);
	mPlayer->SetLightList(mLightList.get());

	//地形
	mMap = std::make_unique<Map>();
	mMap->Create(dxCommon);

	//天球
	mSkydome = std::make_unique<Skydome>();
	mSkydome->Initialize(dxCommon);

	//クロスヘア
	mCrosshair = std::make_unique<Crosshair>();
	mCrosshair->Initialize(dxCommon);

	//パーティクル
	mParticle = std::make_unique<ParticleList>();
	mParticle->Create(dxCommon);

	//草
	mGrasses.resize(mGRASS_MAX);
	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		mGrasses[i] = std::make_unique<Grass>();
		mGrasses[i]->Initialize(dxCommon);
	}
	mGrasses[0]->SetTranslate({ -12.5f,2.0f,20.0f });

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

	//ジェム
	mGems.resize(1);
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i] = std::make_unique<Gem>();
		mGems[i]->Initialize(dxCommon);
	}
	mGems[0]->SetTranslate({ 27.0f,10.0f,-22.5f });

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

	//はしご
	mLadders.resize(mLADDER_MAX);
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		mLadders[i] = std::make_unique<Ladder>();
		mLadders[i]->Initialize(dxCommon);
	}
	//モデルの作成
	mLadderModel_height15_01 = std::make_unique<Model>();
	mLadderModel_height15_01->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	/*mLadderModel_height15_02 = std::make_unique<Model>();
	mLadderModel_height15_02->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");*/
	//離島にあるはしご
	mLadders[0]->SetScale({ 0.5f,0.5f,0.5f });
	mLadders[0]->SetTranslate({ -3.0f,13.0f,87.5f });
	mLadders[0]->SetHeight(30.0f);
	mLadders[0]->SetDirection(Ladder::RIGHT); //右向き
	//リス地に一番近いはしご
	mLadders[1]->SetModel(mLadderModel_height15_01.get());
	mLadders[1]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[1]->SetTranslate({ -2.5f,10.0f,-22.5f });
	mLadders[1]->SetRotate({ 0.0f,-kPi / 2.0f,0.0f });
	mLadders[1]->SetHeight(15.0f);
	mLadders[1]->SetDirection(Ladder::FRONT); //手前向き
	//クランクを回すためのはしご
	//mLadders[2]->SetModel(mLadderModel_height15_02.get());
	//mLadders[2]->SetScale({ 1.0f,1.0f,1.0f });
	//mLadders[2]->SetTranslate({25.0f,7.5f,0.0f});
	//mLadders[2]->SetHeight(15.0f);
	//mLadders[2]->SetDirection(Ladder::RIGHT); //右向き

	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		switch (mLadders[i]->GetDirection()) {
		case Ladder::FRONT:
			mLadders[i]->SetVec({ 0.0f,0.0f,1.0f });
			break;
		case Ladder::BACK:
			mLadders[i]->SetVec({ 0.0f,0.0f,-1.0f });
			break;
		case Ladder::LEFT:
			mLadders[i]->SetVec({ -1.0f,0.0f,0.0f });
			break;
		case Ladder::RIGHT:
			mLadders[i]->SetVec({ 1.0f,0.0f,0.0f });
			break;
		}
	}

	//Aボタン
	//mAbuttonSprite = std::make_unique<Sprite>();
	//mAbuttonSprite->Create(dxCommon, "resources/Sprite/Ui/Buttons/xbox_button_color_a.png");
	//mAbuttonSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{620.0f,600.0f,0.0f} });

	//クランクスプライト
	//mCrankSprite = std::make_unique<Sprite>();
	//mCrankSprite->Create(dxCommon, "resources/Sprite/Crank/crank.png");
	//mCrankSprite->SetTransform({{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{100.0f,100.0f,0.0f}});

	//クリアテキスト
	mClearSprite = std::make_unique<Sprite>();
	mClearSprite->Create(dxCommon, "resources/Sprite/Text/COURSE_CLEAR.png");
	mClearSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{100.0f,300.0f,0.0f} });
	//ゲームオーバーテキスト
	mGameoverSprite = std::make_unique<Sprite>();
	mGameoverSprite->Create(dxCommon, "resources/Sprite/Text/GAME_OVER.png");
	mGameoverSprite->SetTransform({ {1.0f,1.0f,1.0f,},{0.0f,0.0f,0.0f},{200.0f,300.0f,0.0f} });
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
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		Vector3 pos = mLadders[i]->GetTranslate();
		ImGui::DragFloat3("ladderPos", &pos.x);
		mLadders[i]->SetTranslate(pos);
	}
	ImGui::End();

	//mCrankSprite->Update();
	mClearSprite->Update();
	mGameoverSprite->Update();

	if (mIsDirectionalLight == false) {
		mLightList->SetDirectionalLightIntensity(0.0f);
	}
	else {
		mLightList->SetDirectionalLightIntensity(0.7f);
	}
	Transform spriteTransform = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,2.0f,0.0f} };

	mLightList->Update();
	mPlayer->Update(input, mBirdEyeCamera->GetLon());

	if (mIsPlayerCamera == true) {
		mPlayerCamera->Update();
	}
	else {
		mBirdEyeCamera->Update(input, mPlayer->GetWorldPosition());
	}

	mMap->Update();
	mSkydome->Update();
	mCrosshair->Update();
	mParticle->Update(mPlayerCamera.get());
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		mRotateEnemies[i]->Update();
		mRotateEnemies[i]->TrackPlayer(mPlayer.get());
	}
	for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
		mWalkEnemies[i]->Update();
		mGhosts[i]->TrackPlayer(mPlayer.get());
	}
	for (uint32_t i = 0; i < mGhosts.size(); ++i) {
		mGhosts[i]->Update();
	}
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Update();
	}
	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		mGrasses[i]->Update();
	}

	mStar->Update();
	mSlideFloor->Update();
	mSlideSwitch->Update();
	//mUpFloor->Update();
	//mUpSwitch->Update();
	mCrank->Update(input);
	mRotateFloor->Update();
	mRotateFloor->SetRotate(mCrank->GetRotate());
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		mLadders[i]->Update();
	}

	CollisionResult collisionResult;
	//壁,床とプレイヤーの当たり判定
	mPlayer->SetIsHit(false);
	for (uint32_t i = 0; i < mMap->GetBlock().size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mMap->GetBlock()[i]->GetWorldAABB(), collisionResult)) {
			mPlayer->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos += collisionResult.normal * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//rotateEnemyとプレイヤーの当たり判定
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mRotateEnemies[i]->GetAABB(), collisionResult)) {
			mIsGameover = true;
		}
	}

	//壁とrotateEnemyの当たり判定
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

	//ghostとプレイヤーの当たり判定
	for (uint32_t i = 0; i < mGhosts.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mGhosts[i]->GetAABB(), collisionResult)) {
			mIsGameover = true;
		}
	}

	//starとプレイヤーの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mStar->GetAABB(), collisionResult)) {
		mIsClear = true;
	}

	//gemとプレイヤーの当たり判定
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mGems[i]->GetAABB(), collisionResult)) {
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth;
			pos.z += collisionResult.normal.z * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//grassとプレイヤーの当たり判定
	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mGrasses[i]->GetAABB(), collisionResult)) {
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth;
			pos.z += collisionResult.normal.z * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//SlideSwitchとプレイヤーの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mSlideSwitch->GetAABB(), collisionResult)) {
		mSwitchIsHit = true;
		mSlideFloor->SetIsMoving(true);
	}
	else {
		mSwitchIsHit = false;
	}

	//slideFloorとプレイヤーの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mSlideFloor->GetAABB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		mSlideFloorIsHit = true;
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth;
		pos.y += collisionResult.normal.y * collisionResult.depth;
		pos.z += collisionResult.normal.z * collisionResult.depth;
		mPlayer->SetTranslate(pos);
		//mPlayer->CalcurateAABB(mPlayer->GetTranslate());
		if (mPlayer->GetParent() == nullptr) {
			//プレイヤーとslideFloorの親子関係を結ぶ
			Matrix4x4 local = Multiply(mPlayer->GetWorldMatrix(), Inverse(mSlideFloor->GetWorldMatrix()));
			mPlayer->SetTranslate(Vector3{ local.m[3][0],local.m[3][1],local.m[3][2] });
		}
		auto& tmp = mSlideFloor->GetTransform();
		mPlayer->SetParent(&tmp);
	}
	else {
		if (mSlideFloorIsHit == true) {
			mPlayer->SetParent(nullptr);
			mSlideFloorIsHit = false;
			Matrix4x4 world = mPlayer->GetWorldMatrix();
			mPlayer->SetTranslate(Vector3{ world.m[3][0],world.m[3][1],world.m[3][2] });
		}
	}

	//クランクとプレイヤーの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mCrank->GetAABB(), collisionResult)) {
		mCrank->SetIsHit(true);
		mBirdEyeCamera->SetIsHit(true);
	}
	else {
		mCrank->SetIsHit(false);
		mBirdEyeCamera->SetIsHit(false);
	}

	//回転する床とプレイヤーの当たり判定
	if (IsCollision(mPlayer->GetAABB(), mRotateFloor->GetOBB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		Vector3 pos = mPlayer->GetTranslate();
		pos.x += collisionResult.normal.x * collisionResult.depth / 2;
		pos.y += collisionResult.normal.y * collisionResult.depth / 2;
		pos.z += collisionResult.normal.z * collisionResult.depth / 2;
		mPlayer->SetTranslate(pos);
	}

	//プレイヤーとはしごの当たり判定
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mLadders[i]->GetAABB(), collisionResult)) {
			mLadders[i]->SetIsHit(true);
			//mLadderIsHit = true;
			Vector3 pos = mPlayer->GetTranslate();
			pos.x += collisionResult.normal.x * collisionResult.depth / 2;
			pos.y += collisionResult.normal.y * collisionResult.depth / 2;
			pos.z += collisionResult.normal.z * collisionResult.depth / 2;
			mPlayer->SetTranslate(pos);
		}
		else {
			mLadders[i]->SetIsHit(false);
			//mLadderIsHit = false;
		}
	}
	//プレイヤーとはしごが当たっているとき
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		if (mLadders[i]->GetIsHit() == true) {
			//はしごの向き(ベクトル)
			Vector3 ladderVec = mLadders[i]->GetVec();
			//プレイヤーの向き
			Vector3 forwardVec = Multiply(Vector3(0.0f, 0.0f, 1.0f), MakeRotateYMatrix(mPlayer->GetRotate().y));
			//内積を計算
			float dotProduct = Dot(forwardVec, ladderVec);
			if (dotProduct >= 0.9f && (input->PushKey(DIK_W) || input->GetLStick().y >= 0.7f)) {
				switch (mLadders[i]->GetDirection()) {
				case Ladder::FRONT:
					mPlayer->SetTranslate(
						{ mLadders[i]->GetTranslate().x,
						mPlayer->GetTranslate().y,
						mLadders[i]->GetTranslate().z - 1.5f }
					);
					break;
				case Ladder::BACK:
					mPlayer->SetTranslate(
						{ mLadders[i]->GetTranslate().x,
						mPlayer->GetTranslate().y,
						mLadders[i]->GetTranslate().z + 1.5f }
					);
					break;
				case Ladder::LEFT:
					mPlayer->SetTranslate(
						{ mLadders[i]->GetTranslate().x - 1.5f,
						mPlayer->GetTranslate().y,
						mLadders[i]->GetTranslate().z }
					);
					break;
				case Ladder::RIGHT:
					mPlayer->SetTranslate(
						{ mLadders[i]->GetTranslate().x + 1.5f,
						mPlayer->GetTranslate().y,
						mLadders[i]->GetTranslate().z }
					);
					break;
				}
				mPlayer->SetGravity(0.0f);
				Vector3 pos = mPlayer->GetTranslate();
				pos.y += 0.2f;
				mPlayer->SetTranslate(pos);
			}
		}
		else {
			mPlayer->SetGravity(0.05f);
		}
	}

	//プレイヤーの行列を更新
	mPlayer->GetTransform().UpdateMatrix();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
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
	//mUpFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	//mUpSwitch->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mCrank->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mRotateFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mSkydome->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mLadderModel_height15_01->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get(),mLadders[0]->GetTransform());
	//mLadderModel_height15_02->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get(), mLadders[2]->GetTransform());
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
