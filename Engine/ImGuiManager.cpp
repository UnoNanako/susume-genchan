#include "DirectXCommon.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "ImGuiManager.h"
#include "WinApiManager.h"
#include <dxcapi.h>
#include <dxgidebug.h>
#include <format>
#include <fstream>
#include <sstream>
#include <Windows.h>

void ImGuiManager::Initialize(WinApiManager* winApp, DirectXCommon* dxCommon)
{
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	//Win32用初期化
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	auto srvHeap = dxCommon->GetSrvDescriptorHeap();
	//DirectX12用初期化
	ImGui_ImplDX12_Init(
		dxCommon->GetDevice(),
		static_cast<int>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap,
		srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvHeap->GetGPUDescriptorHandleForHeapStart()
	);
	//フォント追加
	ImGuiIO& io = ImGui::GetIO();
	//標準フォントを追加する
	io.Fonts->AddFontDefault();
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
	colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Border
	colors[ImGuiCol_Border] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
	colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.24f };

	// Text
	colors[ImGuiCol_Text] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_CheckMark] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };

	// Popups
	colors[ImGuiCol_PopupBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 0.92f };

	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.54f };
	colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.54f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.13f, 0.13, 0.17, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.24, 0.24f, 0.32f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.22f, 0.27f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
	colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };

	// Seperator
	colors[ImGuiCol_Separator] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
	colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
	colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
	colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.29f };
	colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 0.29f };

	// Docking
	//colors[ImGuiCol_DockingPreview] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };

	auto& style = ImGui::GetStyle();
	style.TabRounding = 4;
	style.ScrollbarRounding = 9;
	style.WindowRounding = 7;
	style.GrabRounding = 3;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ChildRounding = 4;
}

void ImGuiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	//フレームの先頭でImGuiに、ここからフレームが始まる旨を告げる
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void ImGuiManager::End()
{
	//描画前準備
	ImGui::Render();
}

void ImGuiManager::Draw(DirectXCommon* dxCommon)
{
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
