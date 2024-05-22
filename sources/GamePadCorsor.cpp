#include "GamePadCorsor.h"
#include "Input/gamepad.h"
#include "Graphics/graphics.h"
void GamePadCorsor::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	float x, y;
	x = static_cast<float>(graphics.GetWindowSize().cx)/2;
	y = static_cast<float>(graphics.GetWindowSize().cy)/2;
	//�J�[�\���̏����ʒu�ݒ�
	SetCursorPos(x, y);
	//�p�b�h�J�[�\���̏����ʒu
	PadCursorsprPos = { x - 50 ,y - 50 };
	padcursorspr = std::make_unique<sprite>(graphics.GetDevice(), filename);
	padCorsorFlag = false;
	disPlayFlag = false;
}

void GamePadCorsor::Update(float elapsedTime)
{
	gamepad& pad = gamepad::Instance();
	Graphics& graphics = Graphics::Instance();
	currentPadCursorPos = SceneManagement::instance().GetCurrentCursorPosition();
	PadcursorPos = SceneManagement::instance().GetCursorPosition();
	float ax = pad.thumb_state_rx();
	float ay = pad.thumb_state_ry();

	speed = -20;

	PadcursorPos.y += ay * speed;
	PadcursorPos.x += ax * -speed;

	//�p�b�h�̃J�[�\�����s����͈�
	{
		if (PadCursorsprPos.x > 1280)
		{
			PadCursorsprPos.x = 1280;
		}
		else if (PadCursorsprPos.x < 0)
		{
			PadCursorsprPos.x = 0;
		}

		if (PadCursorsprPos.y > 720)
		{
			PadCursorsprPos.y = 720;
		}
		else if (PadCursorsprPos.y < 0)
		{
			PadCursorsprPos.y = 0;
		}
	}

	if (pad.thumb_state_rx() || pad.thumb_state_ry())
	{
		PadCursorsprPos.x -= (currentPadCursorPos.x - PadcursorPos.x);
		PadCursorsprPos.y -= (currentPadCursorPos.y - PadcursorPos.y);
		padCorsorFlag = true;
		disPlayFlag = true;
		timer = timerset;				//�����Ă���Ԃ͒l�����葱����
		SetCursorPos(2000, 2000);		//�}�E�X�J�[�\���̈ʒu���Œ�
	}

	//�Q�[���p�b�h����u���Ă��鎞�Ԃ������Ƃ����ɓ���
	if (timer < 0)
	{
		if (padCorsorFlag)//��x�����}�E�X�J�[�\���ʒu�𒆉��ɖ߂�
		{
			SetCursorPos(static_cast<float>(graphics.GetWindowSize().cx) / 2, static_cast<float>(graphics.GetWindowSize().cy) / 2);
			disPlayFlag = false;			//�Q�[���p�b�h�J�[�\���̕\��OFF
			padCorsorFlag = false;			//������if�ɓ���Ȃ��悤false
		}
		timer = 0;
	}
	
	timer -= elapsedTime;
}

void  GamePadCorsor::Render(RenderContext*rc)
{
	Graphics& graphics = Graphics::Instance();
	gamepad& pad = gamepad::Instance();
	
	graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(1), nullptr, 0xFFFFFFFF);
	graphics.GetDeviceContext()->OMSetDepthStencilState(graphics.GetDepthStencilState(3), 0);
	graphics.GetDeviceContext()->RSSetState(graphics.GetRasterizerState(2));

	if (disPlayFlag)
	padcursorspr->render(graphics.GetDeviceContext(), PadCursorsprPos.x-25, PadCursorsprPos.y-25, 50, 50, 1, 1, 1,1,0);

	graphics.GetDeviceContext()->OMSetDepthStencilState(graphics.GetDepthStencilState(3), 0);
	graphics.GetDeviceContext()->RSSetState(graphics.GetRasterizerState(2));
}

//�J�[�\���Ǝl�p�`�̓����蔻��
bool GamePadCorsor::hitChechLect(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 pos2, DirectX::XMFLOAT2 size1, DirectX::XMFLOAT2 size2)
{
	square box1;
	square box2;
	box1.left = pos1.x;
	box1.right = pos1.x + size1.x;
	box1.top = pos1.y;
	box1.bottom = pos1.y + size1.y;

	box2.left = pos2.x;
	box2.right = pos2.x + size2.x;
	box2.top = pos2.y;
	box2.bottom = pos2.y + size2.y;

	if ((box1.right > box2.left) && (box1.left < box2.right) && (box1.bottom > box2.top) && (box1.top < box2.bottom))
	{
		return true;
	}
	return false;
}