#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "Logger.h"
#include "WinApiManager.h"
#include <cassert>
#include <dxcapi.h>
#include <format>
#include <thread>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

void DirectXCommon::Initialize(WinApiManager* winApiManager)
{
	//FPS固定初期化
	InitializeFixFPS();
	//NULL検出
	assert(winApiManager);
	//メンバ変数に記録
	this->mWinApiManager = winApiManager;
	//DirectXCommon初期化(関数呼び出し)
	CreateDevice();
	CreateCommand();
	CreateSwapChain();
	CreateEachDescriptorHeap();
	CreateRenderTargetView();
	ClearDepthBuffer();
	CreateFence();
	InitializeViewport();
	InitializeScissor();
	CreateCompiler();
}

void DirectXCommon::Finalize()
{
	CloseHandle(mFenceEvent);
	mFence->Release();
	mRtvDescriptorHeap->Release();
	mSwapChainResource[0]->Release();
	mSwapChainResource[1]->Release();
	mSwapChain->Release();
	mCommandList->Release();
	mCommandAllocator->Release();
	mCommandQueue->Release();
	mDevice->Release();
	mDxgiFactory->Release();
}

void DirectXCommon::PreDraw()
{
	//--------------------1. バックバッファのインデックスを取得--------------------
	// これから書き込むバックバッファのインデックスを取得
	mBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

	//--------------------2. リソースバリア(表示→レンダーターゲット)--------------------
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = mSwapChainResource[mBackBufferIndex].Get();
	//遷移前（現在）のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	mCommandList->ResourceBarrier(1, &barrier);

	//--------------------3. レンダーターゲットと深度を設定--------------------
	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	mCommandList->OMSetRenderTargets(1, &mRtvHandles[mBackBufferIndex], false, &dsvHandle);

	//--------------------4. レンダーターゲットと深度をクリア--------------------
	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 青っぽい色。RGBAの順
	mCommandList->ClearRenderTargetView(mRtvHandles[mBackBufferIndex], clearColor, 0, nullptr);
	// 指定した深度で画面全体をクリアする
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	mCommandList->RSSetViewports(1, &mViewport);  // Viewportを設定
	mCommandList->RSSetScissorRects(1, &mScissorRect);    // Scirssorを設定
}

void DirectXCommon::PostDraw()
{
	//FPS固定更新
	UpdateFixFPS();
	//--------------------6. リソースバリア(レンダーターゲット→表示)--------------------
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = mSwapChainResource[mBackBufferIndex].Get();
	//画面に描く処理は全て終わり、画面に映すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	mCommandList->ResourceBarrier(1, &barrier);

	//--------------------7. コマンドをクローズ--------------------
	// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	HRESULT hr = mCommandList->Close();
	assert(SUCCEEDED(hr));

	//--------------------8. コマンドを実行--------------------
	// GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { mCommandList.Get()};
	mCommandQueue->ExecuteCommandLists(1, commandLists);

	//--------------------9. 前後の入れ替え--------------------
	// GPUとOSに画面の交換を行うよう通知する
	mSwapChain->Present(1, 0);

	//--------------------10. GPUの実行を待つ(fenceは実行を待つものではなく、GPUの処理が終わったかを調べるもの)--------------------
	//Fenceの値を更新
	mFenceValue++;
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	mCommandQueue->Signal(mFence.Get(), mFenceValue);
	//Fenceの値が指定したSignal値にたどり着いているか確認する
	//GetCompleatedValueの初期値はFence作成時に渡した初期値
	if (mFence->GetCompletedValue() < mFenceValue)
	{
		//指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
		//イベントを待つ
		WaitForSingleObject(mFenceEvent, INFINITE);
	}

	//--------------------11. コマンドをリセット--------------------
	// 次のフレーム用のコマンドリストを準備
	hr = mCommandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = mCommandList->Reset(mCommandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

//デバイスの初期化
void DirectXCommon::CreateDevice() {
	HRESULT hr;
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif
	//DXGIファクトリーの生成
	//HRESULTはWindows系のエラーコードであり、
	//関数が成功したかどうかをSUCCEEDEDマクロで判定できる
	hr = CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));
	//初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));
	//使用するアダプタ用の変数。最初にnullptrを入れておく
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter = nullptr;
	//良い順にアダプタを頼む
	for (UINT i = 0; mDxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr)); //取得できないのは一大事
		//ソフトウェアアダプタでなければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringの方なので注意
			Logger::Log(std::format(L"Use Adapter:{}\n", adapterDesc.Description));
			break;
		}
		useAdapter = nullptr; //ソフトウェアアダプタの場合は見なかったことにする
	}
	//適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);

	//D3D12Deviceの生成
	mDevice = nullptr;

	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&mDevice));
		//指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(hr)) {
			//生成できたのでログ出力を行ってループを抜ける
			Logger::Log(std::format("FeatureLevels : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスの生成がうまくいかなかったので起動できない
	assert(mDevice != nullptr);
	Logger::Log("Complete create D3D12Device!!!\n"); //初期化完了のログを出す

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(mDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//ヤバイエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			//Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			//https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		//解放
		infoQueue->Release();
	}
#endif
}

//コマンド関連の初期化
void DirectXCommon::CreateCommand(){
	//コマンドキューを生成する
	mCommandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	HRESULT hr = mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mCommandQueue));
	//コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
	//コマンドアロケータを生成する
	mCommandAllocator = nullptr;
	hr = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));
	//コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
	//コマンドリストを生成する
	mCommandList = nullptr;
	hr = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList));
	//コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
}

void DirectXCommon::CreateSwapChain(){
	//スワップチェーンを生成する
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WinApiManager::kClientWidth; //画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Height = WinApiManager::kClientHeight; //画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色の形式
	swapChainDesc.SampleDesc.Count = 1; //マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2; //ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //モニタにうつしたら、中身を破棄
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT hr = mDxgiFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), mWinApiManager->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(mSwapChain.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

//各種デスクリプタヒープを生成
void DirectXCommon::CreateEachDescriptorHeap() {
	//RTV用のヒープでディスクリプタの数は2つ。RTVはShader内で触るものではないので、ShaderVisibleはfalse
	mRtvDescriptorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	//SRV用のヒープでディスクリプタの数は128。SRVはShader内で触るものなので、ShaderVisibleはtrue
	mSrvDescriptorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
	//DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	mDsvDescriptorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	mDescriptorSizeSRV = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	mDescriptorSizeRTV = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDescriptorSizeDSV = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

//ディスクリプタヒープを生成する関数
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible){
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = mDevice->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

//レンダーターゲットビューの初期化
void DirectXCommon::CreateRenderTargetView() {
	//SwapChainからResourceを引っ張ってくる
	HRESULT hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&mSwapChainResource[0]));
	//うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = mSwapChain->GetBuffer(1, IID_PPV_ARGS(&mSwapChainResource[1]));
	assert(SUCCEEDED(hr));
	// RTVの設定
	mRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 出力結果をSRGBに変換して書き込む
	mRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2dテクスチャとして書き込む
	// ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = mRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// まず1つ目を作る。1つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	mRtvHandles[0] = rtvStartHandle;
	mDevice->CreateRenderTargetView(mSwapChainResource[0].Get(), &mRtvDesc, mRtvHandles[0]);
	// 2つ目のディスクリプタハンドルを得る（自力で）
	mRtvHandles[1].ptr = mRtvHandles[0].ptr + mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// 2つ目を作る
	mDevice->CreateRenderTargetView(mSwapChainResource[1].Get(), &mRtvDesc, mRtvHandles[1]);
}

void DirectXCommon::ClearDepthBuffer() {
	//生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = WinApiManager::kClientWidth; // Textureの幅
	resourceDesc.Height = WinApiManager::kClientHeight; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行き or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	//深度値のクリア設定	
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; //1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //フォーマット。Resourceと合わせる

	// Resourceの生成
	mDepthBuffer = nullptr;
	HRESULT hr = mDevice->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&resourceDesc,  // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&mDepthBuffer)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	//heap上にDSVを構築する
	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; //2dTexture
	//DSCHeapの先頭にDSVを作る
	mDevice->CreateDepthStencilView(mDepthBuffer.Get(), &dsvDesc, mDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::CreateFence(){
	//FenceとEventを生成する
	//初期値0でFenceを作る
	mFence = nullptr;
	mFenceValue = 0;
	HRESULT hr = mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	assert(SUCCEEDED(hr));

	//FenceのSignalを待つためのイベントを作成する
	mFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(mFenceEvent != nullptr);
}

void DirectXCommon::InitializeViewport(){
	// クライアント領域のサイズと一緒にして画面全体に表示
	mViewport.Width = WinApiManager::kClientWidth;
	mViewport.Height = WinApiManager::kClientHeight;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

void DirectXCommon::InitializeScissor(){
	// 基本的にビューポートと同じ矩形が構成されるようにする
	mScissorRect.left = 0;
	mScissorRect.right = WinApiManager::kClientWidth;
	mScissorRect.top = 0;
	mScissorRect.bottom = WinApiManager::kClientHeight;
}

void DirectXCommon::CreateCompiler(){
	//dxcCompilerを初期化
	mDxcUtils = nullptr;
	mDxcCompiler = nullptr;
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&mDxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&mDxcCompiler));
	assert(SUCCEEDED(hr));

	//現時点ではincludeはしないが、includeに対応するための設定を行っておく
	mIncludeHandler = nullptr;
	hr = mDxcUtils->CreateDefaultIncludeHandler(&mIncludeHandler);
	assert(SUCCEEDED(hr));
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index){
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXCommon::GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index){
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

void DirectXCommon::InitializeFixFPS()
{
	//現在時間を記録する
	mReference = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	//1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	//1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));
	//現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - mReference);
	//1/60秒（よりわずかに短い時間）経っていない場合
	if (elapsed < kMinTime) {
		//1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - mReference < kMinTime) {
			//1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//現在の時間を記録する
	mReference = std::chrono::steady_clock::now();
}

Microsoft::WRL::ComPtr<IDxcBlob> DirectXCommon::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* IDxcIncludeHandler)
{
	//1.hlslファイルを読む
	//これからシェーダーをコンパイルする旨をログに出す
	Logger::Log(std::format(L"Begin CompileShader, path:{},profile:{}\n", filePath, profile));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; //UTF8の文字コードであることを通知

	//2. Compileする
	LPCWSTR arguments[] = {
		filePath.c_str(), //コンパイル対象のhlslファイル名
		L"-E",L"main", //エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile, //ShaderProfileの設定
		L"-Zi",L"-Qembed_debug", //デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr", //メモリレイアウトは行優先
	};
	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer, //読み込んだファイル
		arguments, //コンパイルオプション
		_countof(arguments), //コンパイルオプションの数
		IDxcIncludeHandler, //includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult) //コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	//3. 警告・エラーが出ていないか確認する
	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Logger::Log(shaderError->GetStringPointer());
		//警告・エラーダメゼッタイ
		assert(false);
	}

	//4. Compile結果を受け取って返す
	//コンパイル結果から実行用のバイナル部分を取得
	Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Logger::Log(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetCPUDescriptorHandle()
{
	++mIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (mDescriptorSizeSRV * mIndex);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXCommon::GetGPUDescriptorHandle()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (mDescriptorSizeSRV * mIndex);
	return handleGPU;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DirectXCommon::CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, size_t sizeInBytes) {
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes;// リソースのサイズ。今回はVector4を3頂点分
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

