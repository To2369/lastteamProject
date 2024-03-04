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


	//if (nextScene != nullptr)
	//{
	//	delete nextScene;
	//	nextScene = nullptr;
	//}
	//if (currentScene != nullptr)
	//{
	//	delete currentScene;
	//	currentScene = nullptr;
	//}
};

void SceneManagement::initialize() {}

void SceneManagement::update(ID3D11Device* device,float elapsed_time,float x,float y)
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
			currentScene->initialize(device,x,y);
		}
	}

	if (currentScene != nullptr)
	{
		currentScene->update(elapsed_time, device, x, y);
	}
	
}

void SceneManagement::render(float elapsed_time,RenderContext& rc)
{	
	if (currentScene != nullptr)
	{
		currentScene->render(elapsed_time, rc);
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
	if (currentScene != nullptr)
	{
		delete currentScene;
		currentScene = nullptr;
	}
}



