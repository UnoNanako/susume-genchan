#pragma once
#include <d3d12.h>
#include <wrl.h>
class WinApiManager;
class DirectXCommon;

using namespace Microsoft::WRL;

class ImGuiManager
{
public:
	void Initialize(WinApiManager* winApp, DirectXCommon* dxCommon);
	void Finalize();
	void Begin();
	void End();
	void Draw(DirectXCommon* dxCommon);
};

