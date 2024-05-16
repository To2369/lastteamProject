#include "scene_title.h"
#include "camera.h"
#include "scene_game.h"
#include "scene_loading.h"
#include"Scene_Stage_Select.h"
#include "Input/gamepad.h"
#include "Graphics/graphics.h"
#include"UIManajer.h"
using namespace std;
SceneTitle::~SceneTitle()
{
	UIManager& ince = UIManager::incetance();
	ince.Clear();
}
void SceneTitle::initialize()
{
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	float x, y;
	x = static_cast<float>(graphics.GetWindowSize().cx);
	y = static_cast<float>(graphics.GetWindowSize().cy);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(90),
		x / y,
		0.1f,
		1000.0f
	);
	camera_controller = std::make_unique<CameraController>();
	//定数バッファ生成
	{
		scene_data = std::make_unique<constant_buffer<scene_constants>>(graphics.GetDevice());
		parametric_constant = std::make_unique<constant_buffer<parametric_constants>>(graphics.GetDevice());
	}

	//framebuffer生成
	{
		uint32_t width = static_cast<uint32_t>(graphics.GetWindowSize().cx);
		uint32_t height = static_cast<uint32_t>(graphics.GetWindowSize().cy);
		framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
		framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), width / 2, height / 2, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
	}

	if (GetAsyncKeyState(VK_LBUTTON))
	{
	}
	vector<unique_ptr<UI>>UIs;
	wstring filenam = L"";
	unique_ptr<UI>ui;
	XMFLOAT2 scale{264.928f,101.677f};

	filenam = failepath::UI_Path_Wstring + L"TitleBack.png";
	ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
	ui->SetScale({ 1280.f,720.f });
	ui->SetPosition({});
	ui->SetID(UI_StringID::UI_ID::Title_ID::Back);
	ui->SetHanteiFlag(false);
	UIs.push_back(move(ui));
	ui = nullptr;

	filenam = failepath::UI_Bottun_Other_Path_Wstring + L"StartButton.png";
	ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
	ui->SetScale(scale);
	ui->SetPosition({ 898.248f,365.087 });
	ui->SetID(UI_StringID::UI_ID::Title_ID::Start);
	ui->SetHanteiFlag(true);
	UIs.push_back(move(ui));
	ui = nullptr;

	filenam = failepath::UI_Bottun_Other_Path_Wstring + L"EndButton.png";
	ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
	ui->SetScale(scale);
	ui->SetPosition({ 898.248f,550.694 });
	ui->SetID(UI_StringID::UI_ID::Title_ID::End);
	ui->SetHanteiFlag(true);
	UIs.push_back(move(ui));
	ui = nullptr;
	UIManager& ince = UIManager::incetance();
	ince.UI_move(move(UIs));
	ince.CreateCanbas();
	UIs.clear();

}

void SceneTitle::update(float elapsed_time)
{
	gamepad& pad = gamepad::Instance();
	camera_controller->AddAngle({ pad.thumb_state_ry() * 2.0f * elapsed_time,-pad.thumb_state_rx() * 2.0f * elapsed_time,0 });
	camera_controller->SetAngle({ 0.0f,-0.0f,0.0f });
	camera_controller->Update(elapsed_time);
	UIManager& ince = UIManager::incetance();
	SHORT keyState = GetAsyncKeyState(VK_LBUTTON);
	bool isKKeyPressed = (keyState & 0x8000) != 0;
	
		//UIManager& ince=UIManager::incetance();
		int canbascount = ince.GetCanBassCount();
		for (int i = 0; i < canbascount; i++)
		{
			CanBas* can = ince.GetCanbas(i);
			int uicount = can->GetUICount();
			for (int j = 0; j < uicount; j++)
			{
				UI* ui = can->GetUI(j);
				if (ui->GetHanteiFlag())
				{
					if (ui->GetID() == UI_StringID::UI_ID::Title_ID::Start)
					{
					
						if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetTexture2DDesc()))
						{
							ui->SetIsMouse(true);
							if (isKKeyPressed && !wasKeyPressed)
							{
							    SceneManagement::instance().SceneChange(
									new SceneLoading(new Scene_Stage_Serect));
							}
							

						}
					}
					if (ui->GetID() == UI_StringID::UI_ID::Title_ID::End)
					{
						if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetTexture2DDesc()))
						{
							ui->SetIsMouse(true);
							if (isKKeyPressed && !wasKeyPressed)
							{
								PostQuitMessage(0);//実行終了
							}
						}
					}
				}
			}
		}
		wasKeyPressed = isKKeyPressed;//今回キーが押されたかどうかを次回で使うために入れておく
		ince.Update(elapsed_time);
		//SceneManagement::instance().SceneChange(new SceneLoading( new SceneGame));
	
#if USE_IMGUI
	ImGui::Begin("sceneTitle");
	ince.Gui();
	ImGui::End();
#endif
}

void SceneTitle::render(float elapsed_time)
{
	RenderContext rc;
	Graphics& graphics = Graphics::Instance();

	graphics.renderinit();

	graphics.GetDeviceContext()->OMSetDepthStencilState(graphics.GetDepthStencilState(0), 1);

	graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);

	D3D11_VIEWPORT viewport;
	UINT num_viewports{ 1 };
	graphics.GetDeviceContext()->RSGetViewports(&num_viewports, &viewport);

	rc.view = Camera::instance().GetView();
	rc.projection = Camera::instance().GetProjection();
	rc.lightDirection = light_direction;
	rc.device = graphics.GetDevice();
	DirectX::XMMATRIX VP = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&rc.view), DirectX::XMLoadFloat4x4(&rc.projection));
	DirectX::XMStoreFloat4x4(&scene_data->data.view_projection, VP);

	scene_data->data.light_direction = rc.lightDirection;
	scene_data->data.camera_position.x = Camera::instance().GetFront().x;
	scene_data->data.camera_position.y = Camera::instance().GetFront().y;
	scene_data->data.camera_position.z = Camera::instance().GetFront().z;
	scene_data->data.camera_position.w = 1.0f;
	FLOAT color[] = { CLEAR_COLOR };
	DirectX::XMStoreFloat4x4(&scene_data->data.view_projection, VP);

	framebuffers[0]->clear(graphics.GetDeviceContext(), color);
	framebuffers[0]->activate(graphics.GetDeviceContext());
	scene_data->activate(graphics.GetDeviceContext(), 1);
	parametric_constant->activate(graphics.GetDeviceContext(), 2);

	//オブジェクト描画
	{
		UIManager& ince = UIManager::incetance();
		ince.Render(&rc);

	}

	scene_data->deactivate(graphics.GetDeviceContext());
	framebuffers[0]->deactivate(graphics.GetDeviceContext());

	graphics.GetBitBlockTransfer()->blit(graphics.GetDeviceContext(), framebuffers[0]->shader_resource_views[0].GetAddressOf(), 0, 1);
}

void SceneTitle::finalize()
{

}

void SceneTitle::setFramebuffer()
{
	Graphics& graphics = Graphics::Instance();
	uint32_t width = static_cast<uint32_t>(graphics.GetWindowSize().cx);
	uint32_t height = static_cast<uint32_t>(graphics.GetWindowSize().cy);
	framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
	framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), width / 2, height / 2, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
}
