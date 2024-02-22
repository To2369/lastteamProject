#pragma once

#include <d3d11.h>
#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <wrl.h>
#include "misc.h"
#include "high_resolution_timer.h"
#include "framebuffer.h"
#include "fullscreen_quad.h"
//todo シーンに持たせるのがいい
#include "scene_management.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

#include <dxgi1_6.h>
#include "constant_buffer.h"

CONST LONG SCREEN_WIDTH{ 1280 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST LPCWSTR APPLICATION_NAME{ L"normal name" };

class framework
{
public:
	CONST HWND hwnd;

	SIZE framebuffer_dimensions;

	Microsoft::WRL::ComPtr<IDXGIAdapter3> adapter3;
	size_t video_memory_usage()
	{
		DXGI_QUERY_VIDEO_MEMORY_INFO video_memory_info;
		adapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &video_memory_info);
		return video_memory_info.CurrentUsage / 1024 / 1024;
	}

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK/*UNIT.32*/, LINEAR_BORDER_WHITE/*UNIT.32*/ };
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[5];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[4];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[4];
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

	std::unique_ptr<framebuffer> framebuffers[8];
	std::unique_ptr<fullscreen_quad> bit_block_transfer;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_views[8];

	void create_swap_chain(IDXGIFactory6* dxgi_factory6);
	void create_states();

public:
	//サイズ変更用関数
	void stylize_window(BOOL fullscreen);


	framework(HWND hwnd,BOOL fullscreen);
	~framework();

	framework(const framework&) = delete;
	framework& operator=(const framework&) = delete;
	framework(framework&&) noexcept = delete;
	framework& operator=(framework&&) noexcept = delete;

	int run()
	{
		MSG msg{};

		if (!initialize())
		{
			return 0;
		}

#ifdef USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device.Get(), immediate_context.Get());
		ImGui::StyleColorsDark();
#endif

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				tictoc.tick();
				calculate_frame_stats();
				update(tictoc.time_interval());
				render(tictoc.time_interval());
				if (GetAsyncKeyState(VK_RETURN) & 1)
				{
					stylize_window(!fullscreen_mode);
				}
			}
		}

#ifdef USE_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif

#if 0
		BOOL fullscreen = 0;
		swap_chain->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			swap_chain->SetFullscreenState(FALSE, 0);
		}
#endif

		return uninitialize() ? static_cast<int>(msg.wParam) : 0;
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
#ifdef USE_IMGUI
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif
		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_ENTERSIZEMOVE:
			tictoc.stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.start();
			break;
		case WM_SIZE:
		{
#if 1
			RECT client_rect{};
			GetClientRect(hwnd, &client_rect);
			on_size_changed(static_cast<UINT64>(client_rect.right - client_rect.left), client_rect.bottom - client_rect.top);
#endif
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

private:
	high_resolution_timer tictoc;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	void calculate_frame_stats()
	{
		if (++frames, (tictoc.time_stamp() - elapsed_time) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}

	BOOL fullscreen_mode{ FALSE };
	BOOL vsync{ FALSE };
	BOOL tearing_supported{ FALSE };

	RECT windowed_rect;
	DWORD windowed_style;
	void on_size_changed(UINT64 width, UINT height); //判定？サイズ変更
};

