#pragma once

#include <d3d11.h>
#include <directxmath.h> 
#include <windows.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <sstream>
#include "fullscreen_quad.h"
#include "ImGuiRenderer.h"

#define CLEAR_COLOR 0.0f,0.0f,1.0f,1.0f

class Graphics
{
public:
	Graphics();
	~Graphics();

	static Graphics& Instance() { return *instance; }

	IDXGIAdapter3* GetAdapter() { return adapter3.Get(); }
	ID3D11Device* GetDevice() { return device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return immediate_context.Get(); }
	IDXGISwapChain1* GetSwapChain() { return swap_chain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() { return render_target_view.Get(); }
	ID3D11RenderTargetView** const GetRenderTargetViewAddressOf() { return render_target_view.GetAddressOf(); }
	ID3D11DepthStencilView* GetDepthStencilView() { return depth_stencil_view.Get(); }

	ID3D11SamplerState* GetSamplerState(size_t state) { return sampler_states[state].Get(); }
	ID3D11DepthStencilState* GetDepthStencilState(size_t state) { return depth_stencil_states[state].Get(); }
	ID3D11BlendState* GetBlendState(size_t state) { return blend_states[state].Get(); }
	ID3D11RasterizerState* GetRasterizerState(size_t state) { return rasterizer_states[state].Get(); }
	ID3D11Buffer* GetBuffer(size_t num) { return constant_buffers[num].Get(); }

	ImGuiRenderer* GetImguiRenderer() { return imguiRenderer.get(); }

	SIZE GetWindowSize() { return framebuffer_dimensions; }
	
	fullscreen_quad* GetBitBlockTransfer() { return bit_block_transfer.get(); }
	
public:
	void renderinit();

private:
	static Graphics* instance;
private:
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

	enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK/*UNIT.32*/, LINEAR_BORDER_WHITE/*UNIT.32*/, COMPARISON_LINEAR_BORDER_WHITE };
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[6];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[4];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[4];
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

	std::unique_ptr<fullscreen_quad> bit_block_transfer;

	std::unique_ptr<ImGuiRenderer> imguiRenderer = nullptr;

	BOOL fullscreen_mode{ FALSE };
	BOOL vsync{ FALSE };
	BOOL tearing_supported{ FALSE };

	RECT windowed_rect;
	DWORD windowed_style;
	void create_swap_chain(IDXGIFactory6* dxgi_factory6);
	void create_states();

public:
	void stylize_window(BOOL fullscreen);

	void on_size_changed(UINT64 width, UINT height);
};