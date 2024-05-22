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
#include "audio.h"

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {};

	//�J�n����
	virtual void initialize() = 0;
	//�X�V����
	virtual void update(float elapsed_time) = 0;
	//�`�揈��
	virtual void render(float elapsed_time) = 0;
	//�I������
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

	enum class SCENE_BGM
	{
		SCENE_CLEAR,
		SCENE_GAME_NOW,
		SCENE_TITLE,
		SCENE_MAX,
	};

	enum class SCENE_SE
	{
		SE_BUTTON_DECISION,
		SE_EXTRACTION,
		SE_INJECTION_SOUND,
		SE_LIFT_SOUND_EFFECTS,
		SE_OBJECT_MOVE,
		SE_SWITCH,
		SE_SYRINGE_INTERCALATE,
		SE_MAX,
	};

	void SetCursorPos(const DirectX::XMFLOAT2 pos) { CursorPos = pos; }
	const DirectX::XMFLOAT2 GetCursorPosition() const { return CursorPos; }
	void SetCurrentCursorPos(const DirectX::XMFLOAT2 pos) { CurrentCursorPos = pos; }
	const DirectX::XMFLOAT2 GetCurrentCursorPosition() const { return CurrentCursorPos; }
	void SetWindowPos(const DirectX::XMFLOAT2 pos) { WindowPos = pos; }
	const DirectX::XMFLOAT2 GetWindowPosition() const { return WindowPos; }
	void SetFrameBuffer() { currentScene->setFramebuffer(); }

	audio* GetBgm(int index) { return bgm[index].get(); }
	audio* GetSe(int index) { return se[index].get(); }

private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
	DirectX::XMFLOAT2 WindowPos = {};
	DirectX::XMFLOAT2 CursorPos = {};
	DirectX::XMFLOAT2 CurrentCursorPos = {};

	Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
	IXAudio2MasteringVoice* master_voice = nullptr;
	std::unique_ptr<audio> bgm[static_cast<int>(SCENE_BGM::SCENE_MAX)] = {};
	std::unique_ptr<audio> se[static_cast<int>(SCENE_SE::SE_MAX)] = {};
};
