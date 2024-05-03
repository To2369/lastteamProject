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
#include "Graphics/RenderContext.h"

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

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {};

	//開始処理
	virtual void initialize() = 0;
	//更新処理
	virtual void update(float elapsed_time) = 0;
	//描画処理
	virtual void render(float elapsed_time) = 0;
	//終了処理
	virtual void finalize() = 0;

	
	bool IsReady() { return ready; }
	void SetReady() { ready = true; }
	virtual void setFramebuffer() = 0;
protected:
	bool ready = false;
	std::unique_ptr<framebuffer> framebuffers[8] = {};
};

class SceneManagement
{
public:
	SceneManagement();
	~SceneManagement();

	static SceneManagement& instance()
	{
		static SceneManagement ins;
		return ins;
	}
private:
	void Clear()
	{
		if (currentScene != nullptr)
		{
			currentScene->finalize();
			delete currentScene;
			currentScene = nullptr;
		}
	}

public:
	void initialize();
	void update(float);
	void render(float elapsed_time);
	void finalize();

	void SceneChange(Scene* scene) { 
		nextScene = scene;
	}

	void SetCursorPos(const DirectX::XMFLOAT2 pos) { CursorPos = pos; }
	const DirectX::XMFLOAT2 GetCursorPosition() const { return CursorPos; }
	void SetCurrentCursorPos(const DirectX::XMFLOAT2 pos) { CurrentCursorPos = pos; }
	const DirectX::XMFLOAT2 GetCurrentCursorPosition() const { return CurrentCursorPos; }
	void SetWindowPos(const DirectX::XMFLOAT2 pos) { WindowPos = pos; }
	const DirectX::XMFLOAT2 GetWindowPosition() const { return WindowPos; }
	void SetFrameBuffer() { currentScene->setFramebuffer(); }
private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
	DirectX::XMFLOAT2 WindowPos = {};
	DirectX::XMFLOAT2 CursorPos = {};

	DirectX::XMFLOAT2 CurrentCursorPos = {};
};