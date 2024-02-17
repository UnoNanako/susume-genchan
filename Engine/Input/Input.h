#pragma once
#include <windows.h>
#include <Xinput.h>
#include <cstdint>
#define DIRECTINPUT_VERSION	    0x0800 //DirectInputのバージョン指定
#include <dinput.h>
#include <wrl.h>
#include "MyMath.h"

class WinApiManager;

//入力
class Input
{
public: //メンバ関数
	//キーボード
	//初期化
	void Initialize(WinApiManager* winApiManager);
	//更新
	void Update();
	//キーが押されているか関数
	bool PushKey(BYTE keyNumber);
	//キーがトリガーか関数
	bool TriggerKey(BYTE keyNumber);

	//ゲームパッド
	bool GetButton(int button);
	bool GetButtonUp(int button);
	bool GetButtonDown(int button);
	Vector2 GetLStick();
	Vector2 GetRStick();
	float GetLTrigger();
	float GetRTrigger();

private: //メンバ変数
	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard = nullptr;
	//全キーの入力状態を取得する
	BYTE keys[256] = {};
	BYTE preKeys[256] = {};
	//WindowsAPI
	WinApiManager* winApiManager;

	//ゲームパッド
	XINPUT_STATE mCurr;
	XINPUT_STATE mPrev;
};
