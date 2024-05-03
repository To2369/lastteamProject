#pragma once

#include <d3d11.h>
#include <windows.h>

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

class ImGuiRenderer
{
private:
public:
	ImGuiRenderer(HWND hwnd)  {}
	~ImGuiRenderer() {}

	bool init(ID3D11Device* device, ID3D11DeviceContext* immediate_context)
	{
		HWND hwnd = GetActiveWindow();
#ifdef USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device, immediate_context);
		ImGui::StyleColorsDark();
		return true;
#endif
		return false;
	}

	bool updateInit()
	{
#ifdef USE_IMGUI
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		return true;
#endif
		return false;
	}

	void render()
	{
#ifdef USE_IMGUI
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	}

	bool uninit()
	{
#ifdef USE_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		return true;
#endif
		return false;
	}
};