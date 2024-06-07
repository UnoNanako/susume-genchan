#include "PlayerCamera.h"
#include "Light/DirectionalLight.h"
#include "DirectXCommon.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "Game.h"
#include "ImGuiManager.h"
#include "Input/Input.h"
#include "Logger.h"
#include "Material.h"
#include "3D/Model.h"
#include "MyMath.h"
#include "Particle/Particle.h"
#include "3D/SphereModel.h"
#include "2D/Sprite.h"
#include "2D/SpriteCommon.h" 
#include "StringUtility.h"
#include "2D/Texture.h"
#include "VertexData.h"
#include "WinApiManager.h"
#include <cstdint>
#include <dxcapi.h>
#include <dxgidebug.h>
#include <format>
#include <fstream>
#include <numbers>
#include <random>
#include <sstream>
#include <string>
#include <Windows.h>
#include <wrl.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

using namespace std;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	Game* game = new Game;
	game->Initialize();

	MSG msg{};
	//--------------------10. GPUの実行を待つ(fenceは実行を待つものではなく、GPUの処理が終わったかを調べるもの)	//ウィンドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windoeにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			///--------------------更新処理ここから--------------------

			game->Update();

			///--------------------更新処理ここまで--------------------
			
			//--------------------5. 描画コマンド--------------------
			//ImGuiを描画する
			//描画用のDescriptorHeapの設定
			ID3D12DescriptorHeap* descriptorHeaps[] = { game->GetDxCommon()->GetSrvDescriptorHeap() };
			game->GetDxCommon()->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
			
			//modelCommon->Bind(game->GetDxCommon());
			//particleCommon->Bind(game->GetDxCommon());

			game->Draw();	
		}
	}
	
	game->Finalize();
	delete game;
	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	//リソースチェック
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}
	return 0;
}