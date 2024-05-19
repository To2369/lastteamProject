#include <dwmapi.h>
#include <dxgi.h>
#include "../DirectXTK-main/Src/PlatformHelpers.h"

#include "framework.h"
#include "Graphics/shader.h"
#include "texture.h"
#include "camera.h"
#include "scene_title.h"
#include "Graphics/graphics.h"
#include "Graphics/RenderContext.h"
#include"Graphics/DebugRenderer.h"
#include "variable_management_class_for_hit_test.h"

framework::framework(BOOL fullscreen)
	:graphics()
{
}

//‰Šú‰»ˆ—
bool framework::initialize()
{
	graphics.GetImguiRenderer()->init(graphics.GetDevice(),graphics.GetDeviceContext());
	SceneManagement::instance().SceneChange(new SceneTitle);
	return true;
}

//XVˆ—
void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	graphics.GetImguiRenderer()->updateInit();
	HWND hwnd = GetActiveWindow();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	SceneManagement::instance().SetWindowPos({ static_cast<float>(rect.left),static_cast<float>(rect.top) });

	POINT cursor_pos;
	GetCursorPos(&cursor_pos);
	ScreenToClient(hwnd, &cursor_pos);
	SceneManagement::instance().SetCurrentCursorPos({ static_cast<float>(cursor_pos.x),static_cast<float>(cursor_pos.y) });

	D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	UINT viewport_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	graphics.GetDeviceContext()->RSGetViewports(&viewport_count, viewports);
	VMCFHT::instance().SetViewPort(viewports[0]);
	SceneManagement::instance().update(elapsed_time);

	GetCursorPos(&cursor_pos);
	ScreenToClient(hwnd, &cursor_pos);
	SceneManagement::instance().SetCursorPos({ static_cast<float>(cursor_pos.x),static_cast<float>(cursor_pos.y) });

#ifdef USE_IMGUI
	ImGui::Begin("framework");

	ImGui::End();
#endif
}

//•`‰æˆ—
void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	HRESULT hr{ S_OK };

	SceneManagement::instance().render(elapsed_time);
	
	graphics.GetImguiRenderer()->render();

	hr = graphics.GetSwapChain()->Present(0, 0);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

bool framework::uninitialize()
{
	graphics.GetImguiRenderer()->uninit();
	SceneManagement::instance().finalize();
	return true;
}

framework::~framework()
{
}
