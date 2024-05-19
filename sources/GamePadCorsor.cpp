#include "GamePadCorsor.h"
#include "Input/gamepad.h"
#include "Graphics/graphics.h"
void GamePadCorsor::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	float x, y;
	x = static_cast<float>(graphics.GetWindowSize().cx)/2;
	y = static_cast<float>(graphics.GetWindowSize().cy)/2;
	//カーソルの初期位置設定
	PadCursorsprPos.x = x-50;
	PadCursorsprPos.y = y-50;
	SetCursorPos(x, y);
	padcursorspr = std::make_unique<sprite>(graphics.GetDevice(), Padcoursorfilename);
}

void GamePadCorsor::Update()
{
	gamepad& pad = gamepad::Instance();
	Graphics& graphics = Graphics::Instance();
	currentPadCursorPos = SceneManagement::instance().GetCurrentCursorPosition();
	PadcursorPos = SceneManagement::instance().GetCursorPosition();

	float ax = pad.thumb_state_rx();
	float ay = pad.thumb_state_ry();

	speed = -20;

	PadCoursorvelocity.y += ay * speed;
	PadCoursorvelocity.x += ax * -speed;

	if (PadCursorsprPos.x > 1281)
	{
		PadCursorsprPos.x = 1280;
		if(PadCoursorvelocity.x<0);
		{
			speed = 0;
		}
	}
	if (pad.thumb_state_rx() || pad.thumb_state_ry())
	{
		PadCoursorvelocity.x = 0;
		if (resetFlag)
		{
			PadCursorsprPos.x = (PadcursorPos.x + PadCoursorvelocity.x);
			PadCursorsprPos.y = (PadcursorPos.y + PadCoursorvelocity.y);
		}
	}
#if USE_IMGUI
	ImGui::Begin("GameMouseCursorspr");
	ImGui::InputFloat2("gamecursorPos", &PadcursorPos.x);
	ImGui::InputFloat2("mousevelocity", &PadCoursorvelocity.x);
	ImGui::InputFloat2("sprPos", &PadCursorsprPos.x);
	ImGui::End();
#endif
}

void  GamePadCorsor::Render(RenderContext*rc)
{
	Graphics& graphics = Graphics::Instance();
	padcursorspr->render(graphics.GetDeviceContext(), PadCursorsprPos.x-50, PadCursorsprPos.y-50, 100, 100, 1, 1, 1, 0.5, 0,0,0,50,50);
}