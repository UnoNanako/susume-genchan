#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include "WinApiManager.h"
#include <dxcapi.h>
#include <string>
#include <chrono>

//DirectX基盤
class DirectXCommon
{
public:
	void Initialize(WinApiManager* winApiManager);// 初期化
	void Finalize(); //Initializeの逆
	void PreDraw();// 描画前
	void PostDraw();// 描画後

	//シェーダーコンパイル関数
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(//CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* IDxcIncludeHandler);
	//リソースを作る関数
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, size_t sizeInBytes);
	//デスクリプタヒープの添え字をずらす関数
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle();
	//deviceゲッター
	ID3D12Device* GetDevice() { return mDevice.Get(); }
	IDxcUtils* GetUtils() { return dxcUtils.Get(); }
	IDxcCompiler3* GetCompiler() { return dxcCompiler.Get(); }
	IDxcIncludeHandler* GetHandler() {return includeHandler.Get(); }
	ID3D12DescriptorHeap* GetRtvDescriptorHeap() { return mRtvDescriptorHeap.Get(); }
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return mSrvDescriptorHeap.Get(); }
	ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return mDsvDescriptorHeap.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return mCommandList.Get(); }
	//バックバッファの数を取得
	size_t GetBackBufferCount() const { return swapChainResource.size(); }
private:
	//メンバ変数
	//DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> mDxgiFactory;
	//DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	//コマンドアロケータを生成する
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvDescriptorHeap;
	uint32_t mDescriptorSizeSRV;
	uint32_t mDescriptorSizeRTV;
	uint32_t mDescriptorSizeDSV;


	///
	/// ここから
	///
	//スワップチェーンリソース
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;
	// RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	// ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	uint64_t fenceValue;
	HANDLE fenceEvent;
	UINT backBufferIndex;
	//WindowsAPI
	WinApiManager* winApiManager = nullptr;

	//メンバ関数
	//デバイスの初期化
	void CreateDevice();
	//コマンド関連の初期化
	void CreateCommand();
	//スワップチェーンの生成
	void CreateSwapChain();
	//各種デスクリプタヒープの生成
	void CreateEachDescriptorHeap();
	//ディスクリプタヒープを生成する
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	//レンダーターゲットビューの初期化
	void CreateRenderTargetView();
	//深度バッファの生成
	void ClearDepthBuffer();
	//フェンスの生成
	void CreateFence();
	//ビューポート矩形の初期化
	void InitializeViewport();
	//シザリング矩形の初期化
	void InitializeScissor();
	//DXCコンパイラの生成
	void CreateCompiler();
	//指定番号のCPUデスクリプタハンドルを取得する
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//指定番号のGPUデスクリプタハンドルを取得する
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//index
	uint32_t index;

	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point mReference;
};

