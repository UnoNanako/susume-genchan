#pragma once
#include <Windows.h>
#include <cstdint>

//WindowsAPI
class WinApiManager
{
public: //メンバ関数
	//定数　クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//初期化
	void Initialize();
	//終了
	void Finalize();
	//getter
	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return wc.hInstance; }
private:
	//ウィンドウハンドル
	HWND hwnd = nullptr;
	WNDCLASS wc{};
};

