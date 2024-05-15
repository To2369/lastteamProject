#include "Scene_Stage_Select.h"
#include"UIManajer.h"
#include <Graphics/graphics.h>
#include <camera.h>
#include <camera_controller.h>
#include"scene_game.h"
#include"scene_loading.h"
#include"StageManager.h"
Scene_Stage_Serect::~Scene_Stage_Serect()
{
	UIManager& ince = UIManager::incetance();
	ince.Clear();

}
void Scene_Stage_Serect::initialize()
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

	vector<unique_ptr<UI>>UIs;
	wstring filenam = failepath::UI_Path_Wstring + L"select_stage1.png";
	XMFLOAT2 scale{ 404.495f*0.7f,575.645f };
	unique_ptr<UI>ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
	ui->SetScale(scale);
	ui->SetPosition({101.901f,53.211f});
	ui->SetID(UI_StringID::StageSelect_ID::Stage1);
	ui->SetHanteiFlag(true);
	UIs.push_back(move(ui));
	ui = nullptr;

	filenam = failepath::UI_Path_Wstring + L"select_stage2.png";
	ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
	ui->SetScale(scale);
	ui->SetPosition({519.975f,53.612f});
	ui->SetID(UI_StringID::StageSelect_ID::Stage2);
	ui->SetHanteiFlag(true);
	UIs.push_back(move(ui));
	ui = nullptr;

	filenam = failepath::UI_Path_Wstring + L"select_stage3.png";
	ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
	ui->SetScale(scale);
	ui->SetPosition({916.103f,53.211f});
	ui->SetID(UI_StringID::StageSelect_ID::Stage3);
	ui->SetHanteiFlag(true);
	UIs.push_back(move(ui));
	ui = nullptr;


	UIManager& ince = UIManager::incetance();
	ince.UI_move(move(UIs));
	ince.CreateCanbas(UI_StringID::CanbasID::Menu);
	UIs.clear();
}

void Scene_Stage_Serect::setFramebuffer()
{
	Graphics& graphics = Graphics::Instance();
	uint32_t width = static_cast<uint32_t>(graphics.GetWindowSize().cx);
	uint32_t height = static_cast<uint32_t>(graphics.GetWindowSize().cy);
	framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
	framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), width / 2, height / 2, DXGI_FORMAT_R16G16B16A16_FLOAT, true);


}

void Scene_Stage_Serect::update(float elapsedTime)
{
	UIManager& ince = UIManager::incetance();
	
	StageManager& ince_st = StageManager::incetance();
	SHORT keyState = GetAsyncKeyState(VK_LBUTTON);
	bool isKKeyPressed = (keyState & 0x8000) != 0;
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
				if (ui->GetID() == UI_StringID::StageSelect_ID::Stage1)
				{
					
					if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetScale()))
					{
						ui->SetIsMouse(true);
						if (isKKeyPressed && !wasKeyPressed)
						{
							ince_st.SetStageName(StageName::stage1_1);
							SceneManagement::instance().SceneChange(new SceneLoading(new SceneGame));
						}
					}
				}
				if (ui->GetID() == UI_StringID::StageSelect_ID::Stage2)
				{
					
					if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetScale()))
					{
						ui->SetIsMouse(true);
						if (isKKeyPressed && !wasKeyPressed) 
						{
						
							ince_st.SetStageName(StageName::stage1_2);
							SceneManagement::instance().SceneChange(new SceneLoading(new SceneGame));
						}
						
					}
				}
				if (ui->GetID() == UI_StringID::StageSelect_ID::Stage3)
				{
					
					if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetScale()))
					{
						ui->SetIsMouse(true);
						if (isKKeyPressed && !wasKeyPressed) 
						{
							
							ince_st.SetStageName(StageName::stage1_3);
							SceneManagement::instance().SceneChange(new SceneLoading(new SceneGame));
							
						}
						
					}
				}
			}
		}
	}
	wasKeyPressed = isKKeyPressed;//今回キーが押されたかどうかを次回で使うために入れておく
	ince.Update(elapsedTime);
#if USE_IMGUI
	ImGui::Begin("sceneTitle");
	ince.Gui();
	ImGui::End();
#endif
	GetAsyncKeyState(VK_LBUTTON);
}

void Scene_Stage_Serect::render(float elapsed_time)
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

