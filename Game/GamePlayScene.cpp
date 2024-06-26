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
#include "Engine/3D/Model.h"
#include "Easing.h"

GamePlayScene::GamePlayScene(Game* game) {
	mGame = game;
}

void GamePlayScene::Initialize(DirectXCommon* dxCommon) {
	mDxCommon = dxCommon;
	//ライト
	mLightList = std::make_unique<LightList>();
	mLightList->Create(dxCommon);
	//タイトルシーン
	mTitleScene = std::make_unique<TitleScene>();
	mTitleScene->Initialize(dxCommon);
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
	mGrasses[1]->SetTranslate({ -10.0f,7.5f,122.5f });
	mGrasses[2]->SetTranslate({ -20.0f,7.5f,122.5f });
	//小さい橋
	mMiniBridges.resize(mMINIBRIDGE_MAX);
	for (uint32_t i = 0; i < mMiniBridges.size(); ++i) {
		mMiniBridges[i] = std::make_unique<MiniBridge>();
		mMiniBridges[i]->Initialize(dxCommon);
	}
	mMiniBridges[0]->SetTranslate({ 7.5f,17.0f,-15.0f });
	mMiniBridges[1]->SetTranslate({ 22.5f,17.0f,-15.0f });
	mMiniBridges[2]->SetTranslate({ 2.5f,17.0f,0.0f });
	//フェンス
	mFences.resize(mFENCE_MAX);
	for (uint32_t i = 0; i < mFences.size(); ++i) {
		mFences[i] = std::make_unique<Fence>();
		mFences[i]->Initialize(dxCommon);
	}
	mFences[0]->SetTranslate({ -7.5f,20.0f,-10.5f });
	mFences[1]->SetTranslate({ -2.5f,20.0f,-10.5f });
	mFences[2]->SetTranslate({ 2.5f,20.0f,-10.5f });
	mFences[3]->SetTranslate({ -7.5f,20.0f,-4.5f });
	mFences[3]->SetRotate({ 0.0f,kPi,0.0f });
	mFences[4]->SetTranslate({ -2.5f,20.0f,-4.5 });
	mFences[4]->SetRotate({ 0.0f,kPi,0.0f });
	mFences[5]->SetTranslate({ 32.0f,20.0f,-15.0f });
	mFences[5]->SetRotate({ 0.0f,kPi / 2.0f,0.0f });
	mFences[6]->SetTranslate({ 32.0f,15.0f,-20.0f }); //これ高さが足りないので修正
	mFences[6]->SetRotate({ 0.0f,kPi / 2.0f,0.0f });
	mFences[7]->SetTranslate({ 32.0f,10.0f,-25.0f });
	mFences[7]->SetRotate({ 0.0f,kPi / 2.0f,0.0f });
	mFences[8]->SetTranslate({ 27.5f,10.0f,-30.0f });
	mFences[8]->SetRotate({ 0.0f,kPi,0.0f });
	mFences[9]->SetTranslate({ -2.5f,20.0f,19.5f });
	mFences[10]->SetTranslate({ 2.5f,20.0f,19.5f });
	mFences[11]->SetTranslate({ 7.5f,20.0f,15.0f });
	mFences[11]->SetRotate({ 0.0f,kPi / 2.0f,0.0f });
	mFences[12]->SetTranslate({ 7.5f,20.0f,10.0f });
	mFences[12]->SetRotate({ 0.0f,kPi / 2.0f,0.0f });
	//クランクモデル
	mCrank = std::make_unique<Crank>();

	/// <summary>
	/// スプライト
	/// <summary>
	//クロスヘア
	mCrosshair = std::make_unique<Crosshair>();
	mCrosshair->Initialize(dxCommon);
	//Aボタン
	mAbuttonSprite = std::make_unique<Sprite>();
	mAbuttonSprite->Create(dxCommon, "resources/Sprite/Ui/Buttons/xbox_button_color_a.png");
	mAbuttonSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{620.0f,450.0f,0.0f} });
	//クリアテキスト
	mClearSprite = std::make_unique<Sprite>();
	mClearSprite->Create(dxCommon, "resources/Sprite/Text/GameClear.png");
	mClearSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{105.0f,295.0f,0.0f} });
	//ゲームオーバーテキスト
	mGameoverSprite = std::make_unique<Sprite>();
	mGameoverSprite->Create(dxCommon, "resources/Sprite/Text/GameOver.png");
	mGameoverSprite->SetTransform({ {1.0f,1.0f,1.0f,},{0.0f,0.0f,0.0f},{105.0f,295.0f,0.0f} });
	//NowLoading
	mNowLoadingSprite = std::make_unique<Sprite>();
	mNowLoadingSprite->Create(dxCommon, "resources/Sprite/Text/NowLoading.png");
	mNowLoadingSprite->SetTransform({ { 1.0f,1.0f,1.0f },{0.0f,0.0f,0.0f},{-1280.0f,0.0f,0.0f} });
	//ゲームルール
	mGameRuleSprite = std::make_unique<Sprite>();
	mGameRuleSprite->Create(dxCommon, "resources/Sprite/Text/GameRule.png");
	mGameRuleSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
	//Yボタン
	mYButtonSprite = std::make_unique<Sprite>();
	mYButtonSprite->Create(dxCommon, "resources/Sprite/Text/YButton.png");
	mYButtonSprite->SetTransform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1024.0f,656.0f,0.0f} });

	//イージング
	mInStart = { -1280.0f,0.0f,0.0f };
	mInEnd = { 0.0f,0.0f,0.0f };
	mOutStart = { 0.0f,0.0f,0.0f };
	mOutEnd = { 1280.0f,0.0f,0.0f };
	mInT = 0.0f;
	mOutT = 1.0f;

	GameInit(dxCommon);
}

void GamePlayScene::Finalize() {
	mMap->Finalize();
}

void GamePlayScene::Update(Input* input) {
	switch (mScene) {
	case GAME:
		mIsPushAButton = false;
		//NowLoadingイージング処理
		mOutT += 0.05f;
		if (mOutT >= 1.0f) {
			mOutT = 1.0f;
			if (input->GetButton(XINPUT_GAMEPAD_A) || input->PushKey(DIK_SPACE)) {
				mIsTitleScene = false;
				mPlayer->SetIsOperatable(true);
			}
		}
		mNowLoadingSprite->SetTranslate(Leap(mOutStart, mOutEnd, Easing::EaseInOutCubic(mOutT)));
		mNowLoadingSprite->Update();
		ObjectUpdate(input); //オブジェクトの更新
		Collision(input); //当たり判定の更新

#ifdef _DEBUG
		ImGui::Begin("Debug");
		ImGui::Checkbox("Clear", &mIsClear);
		ImGui::End();
#endif // DEBUG

		if (mIsClear == true) {
			mScene = CLEAR;
		}
		if (mIsGameover == true) {
			mScene = OVER;
		}
		break;
	case CLEAR:
		if (input->GetButton(XINPUT_GAMEPAD_A) || input->PushKey(DIK_SPACE)) {
			mIsPushAButton = true;
		}
		//NowLoadingイージング処理
		if (mIsPushAButton == true) {
			mInT += 0.05f;
			if (mInT >= 1.0f) {
				mInT = 1.0f;
				mIsTitleScene = true;
				GameInit(mDxCommon);
				mScene = GAME;
				mOutT = 0.0f;
				mNowLoadingSprite->SetTranslate(Leap(mOutStart, mOutEnd, Easing::EaseInOutCubic(mOutT)));
				mNowLoadingSprite->Update();
			} else {
				mNowLoadingSprite->SetTranslate(Leap(mInStart, mInEnd, Easing::EaseInOutCubic(mInT)));
				mNowLoadingSprite->Update(); //NowLoading
			}
		}
		break;
	case OVER:
		if (input->GetButton(XINPUT_GAMEPAD_A) || input->PushKey(DIK_SPACE)) {
			mIsPushAButton = true;
		}
		if (mIsPushAButton == true) {
			mInT += 0.05f;
			if (mInT >= 1.0f) {
				mInT = 1.0f;
				mIsTitleScene = true;
				GameInit(mDxCommon);
				mIsGameover = false;
				mScene = GAME;
				mOutT = 0.0f;
				mNowLoadingSprite->SetTranslate(Leap(mOutStart, mOutEnd, Easing::EaseInOutCubic(mOutT)));
				mNowLoadingSprite->Update();
			} else {
				mNowLoadingSprite->SetTranslate(Leap(mInStart, mInEnd, Easing::EaseInOutCubic(mInT)));
				mNowLoadingSprite->Update(); //NowLoading
			}
		}
		break;
	}

#ifdef _DEBUG
	/// <summary>
	/// ImGui
	/// <summary>
	ImGui::Begin("Debug");
	//DirectionalLightのintensity切り替え
	ImGui::Checkbox("isDirectionalLight", &mIsDirectionalLight);
	Transform transform = mBirdEyeCamera.get()->GetTransform();
	ImGui::DragFloat3("birdEyeCameraPos", &transform.translate.x);
	mBirdEyeCamera.get()->SetTransform(transform);
	ImGui::Checkbox("isPlayerCamera", &mIsPlayerCamera);
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		Vector3 pos = mLadders[i]->GetTranslate();
		ImGui::DragFloat3("ladderPos", &pos.x);
		mLadders[i]->SetTranslate(pos);
	}
	ImGui::End();
#endif // DEBUG

	if (mIsDirectionalLight == false) {
		mLightList->SetDirectionalLightIntensity(0.0f);
	} else {
		mLightList->SetDirectionalLightIntensity(0.7f);
	}

	//プレイヤーの行列を更新
	mPlayer->GetTransform().UpdateMatrix();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon) {
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
	} else {
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
	//ジェム
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	//種
	for (uint32_t i = 0; i < mSeeds.size(); ++i) {
		mSeeds[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	//草
	for (uint32_t i = 0; i < mGrasses.size(); ++i) {
		mGrasses[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	//小さい橋
	for (uint32_t i = 0; i < mMiniBridges.size(); ++i) {
		mMiniBridges[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	//フェンス
	for (uint32_t i = 0; i < mFences.size(); ++i) {
		mFences[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	mStar->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mCrank->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mRotateFloor->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mSkydome->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());

	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		mLadders[i]->Draw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	//----------モデルここまで----------

	mGame->GetParticleCommon()->Bind(dxCommon);
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i]->ParticleDraw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	}
	mStar->ParticleDraw(dxCommon->GetCommandList(), mBirdEyeCamera.get());
	mPlayer->ParticleDraw(dxCommon->GetCommandList(), mBirdEyeCamera.get());

	mGame->GetModelCommon()->Bind(dxCommon);
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		if (mIsTitleScene == false) {
			mGems[i]->SpriteDraw(dxCommon->GetCommandList());
		}
	}
	if (mIsTitleScene == false) {
		mPlayer->SpriteDraw(dxCommon->GetCommandList());
	}
	mCrosshair->Draw(dxCommon->GetCommandList());
	if (mIsTitleScene) {
		mTitleScene->Draw(dxCommon->GetCommandList());
	}
	if (mIsClear) {
		mClearSprite->Draw(dxCommon->GetCommandList());
		mAbuttonSprite->Draw(dxCommon->GetCommandList());
	}
	if (mIsPushYButton == true && mIsClear == false) {
		mGameRuleSprite->Draw(dxCommon->GetCommandList());
	}
	if (mIsPushYButton == false) {
		mYButtonSprite->Draw(dxCommon->GetCommandList());
	}
	if (mPlayer->GetHp() <= 0) {
		mIsGameover = true;
		mGameoverSprite->Draw(dxCommon->GetCommandList());
		mAbuttonSprite->Draw(dxCommon->GetCommandList());
	}
	mNowLoadingSprite->Draw(dxCommon->GetCommandList());
}

void GamePlayScene::LadderInitialize(DirectXCommon* dxCommon) {
	//はしご
	mLadders.resize(mLADDER_MAX);
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		mLadders[i] = std::make_unique<Ladder>();
		mLadders[i]->Initialize(dxCommon);
	}
	//モデルの作成
	mLadderModel_height15_01 = std::make_unique<Model>();
	mLadderModel_height15_01->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	mLadderModel_height15_02 = std::make_unique<Model>();
	mLadderModel_height15_02->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	mLadderModel_height15_03 = std::make_unique<Model>();
	mLadderModel_height15_03->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	mLadderModel_height05 = std::make_unique<Model>();
	mLadderModel_height05->Create(dxCommon, "resources/Model/Ladder", "height1.obj");
	mLadderModel_height15_04 = std::make_unique<Model>();
	mLadderModel_height15_04->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	mLadderModel_height15_05 = std::make_unique<Model>();
	mLadderModel_height15_05->Create(dxCommon, "resources/Model/Ladder", "inFront.obj");
	//リス地に一番近いはしご
	mLadders[0]->SetModel(mLadderModel_height15_01.get());
	mLadders[0]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[0]->SetTranslate({ -2.5f,10.0f,-22.5f });
	mLadders[0]->SetRotate({ 0.0f,-kPi / 2.0f,0.0f });
	mLadders[0]->SetHeight(15.0f);
	mLadders[0]->SetDirection(Ladder::FRONT); //手前向き
	//クランクを回すためのはしご
	mLadders[1]->SetModel(mLadderModel_height15_02.get());
	mLadders[1]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[1]->SetTranslate({ 25.0f,10.0f,0.0f });
	mLadders[1]->SetHeight(15.0f);
	mLadders[1]->SetDirection(Ladder::RIGHT); //右向き
	//離島1つめのはしご
	mLadders[2]->SetModel(mLadderModel_height15_03.get());
	mLadders[2]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[2]->SetTranslate({ 22.5f,10.0f,102.5f });
	mLadders[2]->SetHeight(15.0f);
	mLadders[2]->SetDirection(Ladder::RIGHT); //右向き
	//離島高さ1のはしご
	mLadders[3]->SetModel(mLadderModel_height05.get());
	mLadders[3]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[3]->SetTranslate({ -2.0f,5.0f,132.5f });
	mLadders[3]->SetHeight(5.0f);
	mLadders[3]->SetDirection(Ladder::RIGHT); //右向き
	//離島高さ15のはしご
	mLadders[4]->SetModel(mLadderModel_height15_04.get());
	mLadders[4]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[4]->SetRotate({ 0.0f,kPi / 2.0f,0.0f });
	mLadders[4]->SetTranslate({ -16.0f,10.0f,114.0f });
	mLadders[4]->SetHeight(15.0f);
	mLadders[4]->SetDirection(Ladder::BACK); //後ろ向き
	//離島高さ15のはしご(1番ゴールに近い)
	mLadders[5]->SetModel(mLadderModel_height15_05.get());
	mLadders[5]->SetScale({ 1.0f,1.0f,1.0f });
	mLadders[5]->SetRotate({ 0.0f,kPi / 2.0f,0.0f });
	mLadders[5]->SetTranslate({ -16.0f,20.0f,108.0f });
	mLadders[5]->SetHeight(27.5f);
	mLadders[5]->SetDirection(Ladder::BACK); //後ろ向き

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

void GamePlayScene::ObjectUpdate(Input* input) {
	/// <summary>
	/// 更新
	/// <summary>
	//ゲームルールの表示(Yボタンを押したら表示)
	if (mIsPushYButton == false) {
		if (input->GetButtonDown(XINPUT_GAMEPAD_Y)) {
			mIsPushYButton = true;
		}
	} else if (mIsPushYButton == true) {
		if (input->GetButtonDown(XINPUT_GAMEPAD_Y)) {
			mIsPushYButton = false;
		}
	}
	if (mIsPushYButton == false) {
		mPlayer->Update(input, mBirdEyeCamera->GetLon()); //プレイヤー
		mLightList->Update(); //ライト
		//プレイヤーカメラ
		if (mIsPlayerCamera == true) {
			mPlayerCamera->Update();
		} else {
			mBirdEyeCamera->Update(input, mPlayer->GetWorldPosition(), mIsTitleScene);
		}
		mMap->Update(); //マップ
		mSkydome->Update(); //天球

		//回転する敵
		for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
			mRotateEnemies[i]->Update(mPlayer.get());
		}
		//歩く敵
		for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
			mWalkEnemies[i]->Update();
		}
		//ゴースト(テレサ)
		for (uint32_t i = 0; i < mGhosts.size(); ++i) {
			mGhosts[i]->Update();
			if (mIsTitleScene == false) {
				mGhosts[i]->TrackPlayer(mPlayer.get());
			}
		}
		//ジェム
		for (uint32_t i = 0; i < mGems.size(); ++i) {
			mGems[i]->Update();
		}
		//種
		for (uint32_t i = 0; i < mSeeds.size(); ++i) {
			mSeeds[i]->Update();
		}
		//草オブジェクト
		for (uint32_t i = 0; i < mGrasses.size(); ++i) {
			mGrasses[i]->Update();
		}
		//小さい橋
		for (uint32_t i = 0; i < mMiniBridges.size(); ++i) {
			mMiniBridges[i]->Update();
		}
		//フェンス
		for (uint32_t i = 0; i < mFences.size(); ++i) {
			mFences[i]->Update();
		}
		mStar->Update();
		mCrank->Update(input);
		mRotateFloor->Update();
		mRotateFloor->SetRotate(mCrank->GetRotate());
		for (uint32_t i = 0; i < mLadders.size(); ++i) {
			mLadders[i]->Update();
		}
		mClearSprite->Update(); //クリアスプライト
		mGameoverSprite->Update(); //ゲームオーバースプライト
		mCrosshair->Update(); //クロスヘア
		mAbuttonSprite->Update(); //Aボタン
		mGameRuleSprite->Update(); //ゲームルール
		mYButtonSprite->Update();
		if (mIsTitleScene) {
			mTitleScene->Update();
		}
	}
}


void GamePlayScene::Collision(Input* input) {
	CollisionResult collisionResult;
	//壁,床とプレイヤー
	mPlayer->SetIsHit(false);
	for (uint32_t i = 0; i < mMap->GetBlock().size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mMap->GetBlock()[i]->GetWorldAABB(), collisionResult)) {
			if (collisionResult.normal.y >= 0.5f) {
				mPlayer->SetIsHit(true);
			}
			Vector3 pos = mPlayer->GetTranslate();
			pos += collisionResult.normal * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//小さい橋
	for (uint32_t i = 0; i < mMiniBridges.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mMiniBridges[i]->GetAABB(), collisionResult)) {
			mPlayer->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos += collisionResult.normal * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//フェンス
	for (uint32_t i = 0; i < mFences.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mFences[i]->GetAABB(), collisionResult)) {
			Vector3 pos = mPlayer->GetTranslate();
			pos += collisionResult.normal * collisionResult.depth;
			mPlayer->SetTranslate(pos);
		}
	}

	//rotateEnemyとプレイヤー
	for (uint32_t i = 0; i < mRotateEnemies.size(); ++i) {
		if (mRotateEnemies[i]->GetIsAlive() == true) {
			if (IsCollision(mPlayer->GetAABB(), mRotateEnemies[i]->GetAABB(), collisionResult)) {
				if (mPlayer->GetIsAttack() == false) {
					if (mPlayer->GetIsEnemyHit() == false) {
						mPlayer->SetHp(mPlayer->GetHp() - 1);
					}
					mPlayer->SetIsEnemyHit(true);
				}
			}
			if (IsCollision(mPlayer->GetAABB(), mRotateEnemies[i]->GetAABB(), collisionResult)) {
				if (mPlayer->GetIsAttack() == true) {
					mRotateEnemies[i]->SetIsAlive(false);
				}
			}
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

	//walkEnemyとプレイヤー
	for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
		if (mWalkEnemies[i]->GetIsAlive() == true) {
			if (IsCollision(mWalkEnemies[i]->GetAABB(), mPlayer->GetAABB(), collisionResult)) {
				if (mPlayer->GetIsAttack() == false) {
					if (mPlayer->GetIsEnemyHit() == false) {
						mPlayer->SetHp(mPlayer->GetHp() - 1);
					}
					mPlayer->SetIsEnemyHit(true);
				}
			}
			if (IsCollision(mWalkEnemies[i]->GetAABB(), mPlayer->GetAABB(), collisionResult)) {
				if (mPlayer->GetIsAttack() == true) {
					mWalkEnemies[i]->SetIsAlive(false);
				}
			}
		}
	}

	//ghostとプレイヤー
	for (uint32_t i = 0; i < mGhosts.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mGhosts[i]->GetAABB(), collisionResult)) {
			if (mPlayer->GetIsEnemyHit() == false) {
				mPlayer->SetHp(mPlayer->GetHp() - 1);
			}
			mPlayer->SetIsEnemyHit(true);
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
		}
	}

	//種とプレイヤー
	for (uint32_t i = 0; i < mSeeds.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mSeeds[i]->GetAABB(), collisionResult) && mSeeds[i]->GetIsHit() == false) {
			if (mSeeds[i]->GetIsHit() == false) {
				mPlayer->SetAttackTimes(mPlayer->GetmAttackTimes() + 1);
			}
			mSeeds[i]->SetIsHit(true);
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

	//クランクとプレイヤー
	if (IsCollision(mPlayer->GetAABB(), mCrank->GetAABB(), collisionResult)) {
		mCrank->SetIsHit(true);
		mBirdEyeCamera->SetIsHit(true);
	} else {
		mCrank->SetIsHit(false);
		mBirdEyeCamera->SetIsHit(false);
	}

	//プレイヤーとはしご
	for (uint32_t i = 0; i < mLadders.size(); ++i) {
		if (IsCollision(mPlayer->GetAABB(), mLadders[i]->GetAABB(), collisionResult)) {
			mLadders[i]->SetIsHit(true);
			Vector3 pos = mPlayer->GetTranslate();
			pos += collisionResult.normal * collisionResult.depth / 2;
			mPlayer->SetTranslate(pos);
		} else {
			mLadders[i]->SetIsHit(false);
		}
	}

	//回転する床とプレイヤー
	if (IsCollision(mPlayer->GetAABB(), mRotateFloor->GetOBB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		Vector3 pos = mPlayer->GetTranslate();
		pos += collisionResult.normal * collisionResult.depth / 2;
		mPlayer->SetTranslate(pos);
	}
	if (IsCollision(mPlayer->GetAABB(), mRotateFloor->GetSideLOBB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		Vector3 pos = mPlayer->GetTranslate();
		pos += collisionResult.normal * collisionResult.depth / 2;
		mPlayer->SetTranslate(pos);
	}
	if (IsCollision(mPlayer->GetAABB(), mRotateFloor->GetSideROBB(), collisionResult)) {
		mPlayer->SetIsHit(true);
		Vector3 pos = mPlayer->GetTranslate();
		pos += collisionResult.normal * collisionResult.depth / 2;
		mPlayer->SetTranslate(pos);
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
						if (mPlayer->GetTranslate().y >= mLadders[i]->GetHeight()) {
							mPlayer->SetTranslate({ mPlayer->GetTranslate().x,mPlayer->GetTranslate().y + 2.0f,mPlayer->GetTranslate().z + 2.0f });
						}
						break;
					case Ladder::BACK:
						mPlayer->SetTranslate(
							{ mLadders[i]->GetTranslate().x,
							mPlayer->GetTranslate().y,
							mLadders[i]->GetTranslate().z + 1.5f }
						);
						if (mPlayer->GetTranslate().y >= mLadders[i]->GetHeight()) {
							mPlayer->SetTranslate({ mPlayer->GetTranslate().x,mPlayer->GetTranslate().y + 2.0f,mPlayer->GetTranslate().z - 2.0f });
						}
						break;
					case Ladder::LEFT:
						mPlayer->SetTranslate(
							{ mLadders[i]->GetTranslate().x - 1.5f,
							mPlayer->GetTranslate().y,
							mLadders[i]->GetTranslate().z }
						);
						if (mPlayer->GetTranslate().y >= mLadders[i]->GetHeight()) {
							mPlayer->SetTranslate({ mPlayer->GetTranslate().x + 2.0f, mPlayer->GetTranslate().y + 2.0f,mPlayer->GetTranslate().z });
						}
						break;
					case Ladder::RIGHT:
						mPlayer->SetTranslate(
							{ mLadders[i]->GetTranslate().x + 1.5f,
							mPlayer->GetTranslate().y,
							mLadders[i]->GetTranslate().z }
						);
						if (mPlayer->GetTranslate().y >= mLadders[i]->GetHeight()) {
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
	} else {
		mPlayer->SetGravity(0.05f);
	}
}

void GamePlayScene::GameInit(DirectXCommon* dxCommon) {
	mInT = 0.0f; //イージング用
	//カメラの初期化
	mPlayerCamera = std::make_unique<PlayerCamera>();
	mPlayerCamera->Initialize(dxCommon);
	//俯瞰カメラ
	mBirdEyeCamera = std::make_unique<BirdEyeCamera>();
	mBirdEyeCamera->Initialize(dxCommon);

	/// <summary>
	/// プレイヤー
	/// <summary>
	mPlayer = std::make_unique<Player>();
	mPlayer->Initialize(dxCommon);
	mPlayer->SetLightList(mLightList.get());

	/// <summary>
	/// オブジェクト
	/// <summary>
	mCrank->Initialize(dxCommon);
	//ジェム
	mGems.resize(3);
	for (uint32_t i = 0; i < mGems.size(); ++i) {
		mGems[i] = std::make_unique<Gem>();
		mGems[i]->Initialize(dxCommon, { (1088.0f + i * 64.0f),10.0f,0.0f });
	}
	mGems[0]->SetTranslate({ 27.0f,10.0f,-25.0f });
	mGems[1]->SetTranslate({ 17.5f,5.0f,97.5f });
	mGems[2]->SetTranslate({ -16.0f,20.0f,90.75f });
	//スター
	mStar = std::make_unique<Star>();
	mStar->Initialize(dxCommon);
	mStar->SetTranslate({ -15.5f,30.0f,95.0f });
	//種
	mSeeds.resize(mSEED_MAX);
	for (uint32_t i = 0; i < mSEED_MAX; ++i) {
		mSeeds[i] = std::make_unique<Seed>();
		mSeeds[i]->Initialize(dxCommon);
	}
	//本島
	mSeeds[0]->SetTranslate({ -2.5f,5.0f,20.0f });
	mSeeds[1]->SetTranslate({ -2.5f,5.0f,-10.0f });
	mSeeds[2]->SetTranslate({ -2.5f,5.0f,-5.0f });
	//離島
	mSeeds[3]->SetTranslate({ -10.0f,8.5f,132.5f });
	mSeeds[4]->SetTranslate({ -20.0f,8.5f,132.5f });
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
	mRotateEnemies[0]->SetInitTranslate({ 15.0f,2.0f,12.5f });
	mRotateEnemies[1]->SetTranslate({ 12.5f,17.5f,102.5f });
	mRotateEnemies[1]->SetInitTranslate({ 12.5f,17.5f,102.5f });
	//walkEnemy(歩き回る敵)
	mWalkEnemies.resize(mWALKENEMY_MAX);
	for (uint32_t i = 0; i < mWalkEnemies.size(); ++i) {
		mWalkEnemies[i] = std::make_unique<WalkEnemy>();
		mWalkEnemies[i]->Initialize(dxCommon);
	}
	mWalkEnemies[0]->SetTranslate({ -7.5f,2.0f,25.0f });
	mWalkEnemies[0]->SetMoveMax({ -7.5f,0.0f,25.0f });
	mWalkEnemies[0]->SetMoveMin({ -17.5f,0.0f,15.0f });
	mWalkEnemies[1]->SetTranslate({ 7.5f,2.0f,-25.0f });
	mWalkEnemies[1]->SetMoveMax({ 22.5f,0.0f,-10.0f });
	mWalkEnemies[1]->SetMoveMin({ 7.5f,0.0f,-25.0f });
	mWalkEnemies[2]->SetTranslate({ 27.5f,2.0f,5.0f });
	mWalkEnemies[2]->SetMoveMax({ 27.5f,0.0f,5.0f });
	mWalkEnemies[2]->SetMoveMin({ 2.5f,0.0f,-5.0f });
	mWalkEnemies[3]->SetTranslate({ 22.5f,2.0f,90.5f });
	mWalkEnemies[3]->SetMoveMax({ 25.0f,0.0f,114.0f });
	mWalkEnemies[3]->SetMoveMin({ 0.0f,0.0f,90.0f });
	mWalkEnemies[4]->SetTranslate({ -5.0f,7.5f,117.5f });
	mWalkEnemies[4]->SetMoveMax({ -5.0f,0.0f,127.5f });
	mWalkEnemies[4]->SetMoveMin({ -25.0f,0.0f,117.5f });
	mWalkEnemies[4]->SetDirection(WalkEnemy::LEFT);
	mWalkEnemies[5]->SetTranslate({ -27.5f,7.5f,127.5f });
	mWalkEnemies[5]->SetMoveMax({ -5.0f,0.0f,127.5f });
	mWalkEnemies[5]->SetMoveMin({ -25.0f,0.0f,117.5f });
	mWalkEnemies[5]->SetDirection(WalkEnemy::RIGHT);
	mWalkEnemies[6]->SetTranslate({ -21.0f,18.0f,110.5f });
	mWalkEnemies[6]->SetMoveMax({ -10.0f,0.0f,110.0f });
	mWalkEnemies[6]->SetMoveMin({ -22.0f,0.0f,87.5f });
	mWalkEnemies[6]->SetDirection(WalkEnemy::RIGHT);
	//ghost(テレサ)
	mGhosts.resize(mGHOST_MAX);
	for (uint32_t i = 0; i < mGhosts.size(); ++i) {
		mGhosts[i] = std::make_unique<Ghost>();
		mGhosts[i]->Initialize(dxCommon);
	}
	mGhosts[0]->SetTranslate({ 22.5f,5.0f,-5.0f });
	mGhosts[0]->SetRotate({ 0.0f,-kPi / 2.0f,0.0f });

	//フラグ
	mIsClear = false;
}
