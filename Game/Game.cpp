#include "DirectXCommon.h"
#include "Game.h"
#include "GamePlayScene.h"
#include "ImGuiManager.h"
#include "Input/Input.h"
#include "3D/Model.h"
#include "3D/SphereModel.h"
#include "2D/Sprite.h"
#include "2D/SpriteCommon.h"
#include "2D/Texture.h"
#include "Transform.h"
#include "VertexData.h"
#include "WinApiManager.h"
#include "Engine/3D/ModelCommon.h"
#include "Engine/Particle/ParticleCommon.h"

void Game::Initialize()
{
	//ポインタ
	mWinApiManager = nullptr;
	//WindowsAPIの初期化
	mWinApiManager = new WinApiManager();
	mWinApiManager->Initialize();

	//入力の初期化
	mInput = nullptr;
	mInput = new Input();
	mInput->Initialize(mWinApiManager);
	mDxCommon = new DirectXCommon();
	mDxCommon->Initialize(mWinApiManager);

	//スプライト共通部分の初期化
	mSpriteCommon = new SpriteCommon;
	mSpriteCommon->Initialize();

	//Imgui
	mImgui = new ImGuiManager;
	mImgui->Initialize(mWinApiManager, mDxCommon);

	//ゲームプレイシーンの生成
	mScene = new GamePlayScene(this);
	//ゲームプレイシーンの初期化
	mScene->Initialize(mDxCommon);

	mModelCommon = new ModelCommon();
	mParticleCommon = new ParticleCommon();
	mModelCommon->Initialize(mDxCommon);
	mParticleCommon->Initialize(mDxCommon);
}

void Game::Finalize()
{
	//解放処理
	//WindowsAPIの終了処理
	mWinApiManager->Finalize();
	//ImGuiの終了処理
	mImgui->Finalize();
	//ゲームプレイシーンの終了処理
	mScene->Finalize();
	//シーンの解放
	delete mScene;
	delete mInput;
	delete mWinApiManager;
	delete mDxCommon;
	delete mSpriteCommon;
	delete mImgui;
	delete mModelCommon;
	delete mParticleCommon;
}

void Game::Update()
{
	mImgui->Begin();
	//入力の更新
	mInput->Update();
	//キーが押されているときの処理例
			//数字の0キーが押されていたら
	if (mInput->PushKey(DIK_0)) {
		OutputDebugStringA("Hit 0\n");
	}
	//シーンの更新処理
	mScene->Update(mInput);
}

void Game::Draw()
{
	mDxCommon->PreDraw(); //描画前コマンド
	mImgui->End();
	//シーン描画
	mScene->Draw(mDxCommon);
	//実際のcommandListのImGuiの描画コマンドを積む
	mImgui->Draw(mDxCommon);
	mDxCommon->PostDraw(); //描画後コマンド
}
