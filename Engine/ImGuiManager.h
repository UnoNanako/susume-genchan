#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "externals/imgui/imgui_stdlib.h"
#include <magic_enum/magic_enum.hpp>

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

template <class T>
bool Combo(const char* name, T& curr)
{
	bool isChanged = false;
	if (ImGui::BeginCombo(name, magic_enum::enum_name(curr).data())) {
		for (size_t i = 0; i < magic_enum::enum_count<T>(); ++i) {
			bool isSelected = false;
			if (ImGui::Selectable(magic_enum::enum_name(T(i)).data(), isSelected)){
				curr = T(i);
				isChanged = true;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return isChanged;
}
