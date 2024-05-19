#include"GameMouseCursor.h"
#include "Input/gamepad.h"
#include "Graphics/graphics.h"
void GameMauseCorsor::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	float x, y;
	x = static_cast<float>(graphics.GetWindowSize().cx);
	y = static_cast<float>(graphics.GetWindowSize().cy);
	//カーソルの初期位置設定
	mousesprPos.x = x / 2;
	mousesprPos.y = y / 2;
	SetCursorPos(x / 2, y / 2);
	mousespr = std::make_unique<sprite>(graphics.GetDevice(), mousefilename);
}

void GameMauseCorsor::Update()
{
	gamepad& pad = gamepad::Instance();
	Graphics& graphics = Graphics::Instance();
	gamecurrentCursorPos = SceneManagement::instance().GetCurrentCursorPosition();
	gamecursorPos = SceneManagement::instance().GetCursorPosition();

	float ax = pad.thumb_state_rx();
	float ay = pad.thumb_state_ry();

	float speed = -20;

	mousevel.y += ay * speed;
	mousevel.x += ax * -speed;

	if (pad.thumb_state_rx() || pad.thumb_state_ry())
	{
		mousesprPos.x = (gamecursorPos.x + mousevel.x);
		mousesprPos.y = (gamecursorPos.y + mousevel.y);
	}
}

void  GameMauseCorsor::Render(RenderContext*rc)
{
	Graphics& graphics = Graphics::Instance();
	mousespr->render(graphics.GetDeviceContext(), mousesprPos.x, mousesprPos.y, 100, 100, 1, 1, 1, 0.5, 0);
}