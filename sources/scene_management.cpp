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

void SceneManagement::initialize() {}

void SceneManagement::update(float elapsed_time)
{
	if (nextScene != nullptr)
	{
		//�Â��V�[�����I������
		Clear();

		//�V�����V�[����ݒ�
		currentScene = nextScene;
		nextScene = nullptr;

		//�V�[������������
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



