#include <dwmapi.h>
#include <dxgi.h>
#include "scene_management.h"

#include "Graphics/shader.h"
#include "texture.h"

#include "scene_game.h"

#include "Input/gamepad.h"
#include "camera.h"

#include "Graphics/RenderContext.h"



SceneManagement::SceneManagement() {}

SceneManagement::~SceneManagement()
{
	if (currentScene != nullptr)
	{
		currentScene->finalize();
		delete currentScene;
		currentScene = nullptr;
	}
	if (nextScene != nullptr)
	{
		delete nextScene;
		nextScene = nullptr;
	}
};

void SceneManagement::initialize()
{
	HRESULT hr{ S_OK };

	hr = XAudio2Create(xaudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = xaudio2->CreateMasteringVoice(&master_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//BGM設定
	{
		bgm[static_cast<int>(SCENE_BGM::SCENE_CLEAR_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\clear\\clear_jingle1.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_CLEAR_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\clear\\clear_jingle2.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_CLEAR_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\clear\\clear_jingle3.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_GAME_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\game_now\\game_now0.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_GAME_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\game_now\\game_now1.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_GAME_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\game_now\\game_now2.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_GAME_3)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\game_now\\game_now3.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_TITLE_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\title\\title_bgm0.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_TITLE_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\title\\title_bgm1.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_TITLE_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\title\\title_bgm2.wav");
		bgm[static_cast<int>(SCENE_BGM::SCENE_TITLE_3)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\BGM\\title\\title_bgm3.wav");
	}

	//SE設定
	{
		se[static_cast<int>(SCENE_SE::SE_BUTTON_DECISION_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\button\\button_decision_0.wav");
		se[static_cast<int>(SCENE_SE::SE_BUTTON_DECISION_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\button\\button_decision_1.wav");
		se[static_cast<int>(SCENE_SE::SE_BUTTON_DECISION_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\button\\button_decision_2.wav");
		se[static_cast<int>(SCENE_SE::SE_EXTRACTION_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\extraction_0\\extraction_0.wav");
		se[static_cast<int>(SCENE_SE::SE_EXTRACTION_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\extraction_0\\extraction_1.wav");
		se[static_cast<int>(SCENE_SE::SE_EXTRACTION_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\extraction_0\\extraction_2.wav");
		se[static_cast<int>(SCENE_SE::SE_INJECTION_SOUND_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\injection_sound\\injection_sound_0.wav");
		se[static_cast<int>(SCENE_SE::SE_INJECTION_SOUND_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\injection_sound\\injection_sound_1.wav");
		se[static_cast<int>(SCENE_SE::SE_INJECTION_SOUND_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\injection_sound\\injection_sound_2.wav");
		se[static_cast<int>(SCENE_SE::SE_LIFT_SOUND_EFFECTS_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\lift_sound_effects\\lift_sound_effects_0.wav");
		se[static_cast<int>(SCENE_SE::SE_LIFT_SOUND_EFFECTS_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\lift_sound_effects\\lift_sound_effects_1.wav");
		se[static_cast<int>(SCENE_SE::SE_LIFT_SOUND_EFFECTS_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\lift_sound_effects\\lift_sound_effects_2.wav");
		se[static_cast<int>(SCENE_SE::SE_OBJECT_MOVE_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\object_move\\object_move_0.wav");
		se[static_cast<int>(SCENE_SE::SE_OBJECT_MOVE_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\object_move\\object_move_1.wav");
		se[static_cast<int>(SCENE_SE::SE_OBJECT_MOVE_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\object_move\\object_move_2.wav");
		se[static_cast<int>(SCENE_SE::SE_SWITCH_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\switch\\switch_0.wav");
		se[static_cast<int>(SCENE_SE::SE_SWITCH_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\switch\\switch_1.wav");
		se[static_cast<int>(SCENE_SE::SE_SWITCH_2)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\switch\\switch_2.wav");
		se[static_cast<int>(SCENE_SE::SE_SYRINGE_INTERCALATE_0)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\syringe_intercalate\\syringe_intercalate_0.wav");
		se[static_cast<int>(SCENE_SE::SE_SYRINGE_INTERCALATE_1)] = std::make_unique<audio>(xaudio2.Get(), L".\\resources\\audio\\SE\\syringe_intercalate\\syringe_intercalate_1.wav");
	}
}

void SceneManagement::update(float elapsed_time)
{
	if (nextScene != nullptr)
	{
		//古いシーンを終了処理
		Clear();

		//新しいシーンを設定
		currentScene = nextScene;
		nextScene = nullptr;

		//シーン初期化処理
		if (!currentScene->IsReady())
		{
			currentScene->initialize();
		}
	}

	if (currentScene != nullptr)
	{
		currentScene->update(elapsed_time);
	}
	
}

void SceneManagement::render(float elapsed_time)
{	
	if (currentScene != nullptr)
	{
		currentScene->render(elapsed_time);
	}
}

void SceneManagement::finalize()
{

	Clear();
	if (nextScene != nullptr)
	{
		delete nextScene;
		nextScene = nullptr;
	}
}



