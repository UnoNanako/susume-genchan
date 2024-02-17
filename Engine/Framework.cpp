#include "Framework.h"

void Framework::Initialize()
{
}

void Framework::Finalize()
{
}

void Framework::Update()
{
}

void Framework::Run()
{
	//ゲームの初期化
	Initialize();

	while (true) { //ゲームループ
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (IsEndRequst()) {
			break;
		}
		//描画
		Draw();
	}
	//ゲーム終了
	Finalize();
}
