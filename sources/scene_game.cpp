#include "variable_management_class_for_hit_test.h"
#include "scene_game.h"
#include "Input/gamepad.h"
#include "camera.h"
#include"Graphics/DebugRenderer.h"
#include"Gimic.h"
#include"PlayerManager.h"
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif
#include"UIManajer.h"
#include "Graphics/graphics.h"
#include "Graphics/shader.h"
#include"scene_loading.h"
#include"scene_title.h"
#include"scene_stage_select.h"

//
//void GroundRayCamera(XMFLOAT3& pos,XMFLOAT3 Scale,float& velY)
//{
//	StageManager& ince_st = StageManager::incetance();
//	VMCFHT&ince_ray= VMCFHT::instance();
//	XMFLOAT3 normal = {0,-1,0};
//	XMFLOAT3 po = pos;
//	ince_ray.update(po,normal);
//	Object*st = ince_st.GetStages(ince_st.GetNowStage());
//
//	collision_mesh& mesh = *st->GetModel()->Get_RaycastCollition();
//	Intersection inter{};
//	if (ince_ray.raycast(mesh, st->GetTransform(), inter, 50))
//	{
//		pos.y=inter.intersection_position.y+0.2f;
//		vel.y = 0;
//	}
//	pos.y += velY;
//}

void SceneGame::initialize()
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
		0.05f,
		1000.0f
	);
	camera_controller = std::make_unique<CameraController>();

	//定数バッファ生成
	{
		scene_data = std::make_unique<constant_buffer<scene_constants>>(graphics.GetDevice());
		parametric_constant = std::make_unique<constant_buffer<parametric_constants>>(graphics.GetDevice());
	}
	GetAsyncKeyState(VK_RBUTTON);

	//プレイヤー初期設定
	PlayerManager& playerManager = PlayerManager::Instance();
	unique_ptr<Player>pl = make_unique<Player>(graphics.GetDevice());
	pl->SetPosition({ 0,30,0 });
	plm.Register(move(pl));
	{
		//ステージのオブジェクト初期化
		StageManager& ince = StageManager::incetance();
		ince.Initialize_GameStage(ince.GetStageName(), graphics.GetDevice());
	}
	/*Debug_ParameterObj = make_unique<DropBox_Road>(device);
	Debug_ParameterObj->SetPosition({1.f, 0.8f, 0.5f});*/
	Goal_navi = make_unique<Goal_navigation_Arrow>(graphics.GetDevice());
	

	//framebuffer生成
	{
		uint32_t width = static_cast<uint32_t>(graphics.GetWindowSize().cx);
		uint32_t height = static_cast<uint32_t>(graphics.GetWindowSize().cy);
		framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
		framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), width / 2, height / 2, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
		graphics.SetPixelShader(0, "shader\\outline_ps.cso");
	}
	//MenuUI生成
	{
		vector<unique_ptr<UI>>UIs;
		UIManager& ince = UIManager::incetance();
		wstring filenam = failepath::UI_Bottun_Other_Path_Wstring + L"continue.png";
		XMFLOAT2 scale{ 391.f * 0.7f,157.f };
		unique_ptr<UI>ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale(scale);
		ui->SetPosition({533.364f,134.103f});
		ui->SetID(UI_StringID::UI_ID::Menu_Id::MenuContinue);
		ui->SetHanteiFlag(true);
		UIs.push_back(move(ui));
		ui = nullptr;
		filenam = failepath::UI_Bottun_Other_Path_Wstring+L"Ritrai.png";
		ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale(scale);
		ui->SetPosition({ 533.364f,338.952f });
		ui->SetID(UI_StringID::UI_ID::Menu_Id::MenuRitrai);
		ui->SetHanteiFlag(true);
		UIs.push_back(move(ui));
		ui = nullptr;
		filenam = failepath::UI_Bottun_Other_Path_Wstring + L"GibuUp.png";
		ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale(scale);
		ui->SetPosition({ 533.364f,542.577f });
		ui->SetID(UI_StringID::UI_ID::Menu_Id::MenuGibuUp);
		ui->SetHanteiFlag(true);
		UIs.push_back(move(ui));
		ui = nullptr;

		ince.UI_move(move(UIs));
		ince.CreateCanbas(UI_StringID::CanbasID::Menu);
		UIs.clear();

		//MaskSprite = make_unique<sprite>();

	}
	//Clear_ui生成
	{
		vector<unique_ptr<UI>>UIs;
		wstring filenam= L"";
		unique_ptr<UI>ui;
		DirectX::XMFLOAT2 scale{ 391.f,254.f };

		filenam = failepath::UI_Mask_Path_Wstring + L"Black.png";
		ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale({ 1920.f,1080.f });
		ui->SetPosition({});
		ui->SetID(UI_StringID::UI_ID::Mask_Id::Black);
		ui->SetHanteiFlag(false);
		ui->SetColor({ 1.f,1.f,1.f,0.4f });
		UIs.push_back(move(ui));
		ui = nullptr;

		filenam = failepath::UI_Bottun_Other_Path_Wstring + L"ClearLogo.png";
		ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale({753.f,143.f});
		ui->SetPosition({ 270.0f,0.f });
		ui->SetID(UI_StringID::UI_ID::Clear_Id::ClearClearLogo);
		ui->SetHanteiFlag(false);
		UIs.push_back(move(ui));
		ui = nullptr;

		filenam = failepath::UI_Bottun_Other_Path_Wstring + L"NextStage.png";
		ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale(scale);
		ui->SetPosition({ 443.762f,460.044f });
		ui->SetID(UI_StringID::UI_ID::Clear_Id::ClearNextStage);
		ui->SetHanteiFlag(true);
		UIs.push_back(move(ui));
		ui = nullptr;

		filenam = failepath::UI_Bottun_Other_Path_Wstring + L"StageSelectButton.png";
		ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale(scale);
		ui->SetPosition({ 873.201f,460.044f });
		ui->SetID(UI_StringID::UI_ID::Clear_Id::ClearStageSelect);
		ui->SetHanteiFlag(true);
		UIs.push_back(move(ui));
		ui = nullptr;

		filenam = failepath::UI_Bottun_Other_Path_Wstring + L"TitleButton.png";
		ui = make_unique<UI>(graphics.GetDevice(), filenam.c_str());
		ui->SetScale(scale);
		ui->SetPosition({ 17.316f,460.044f });
		ui->SetID(UI_StringID::UI_ID::Clear_Id::ClearTitle);
		ui->SetHanteiFlag(true);
		UIs.push_back(move(ui));
		ui = nullptr;

		
		UIManager& ince = UIManager::incetance();
		ince.UI_move(move(UIs));
		ince.CreateCanbas(UI_StringID::CanbasID::GameClear);
		UIs.clear();
	}
	
}

DirectX::XMFLOAT3 convert_screen_to_world(LONG x/*screen*/, LONG y/*screen*/, FLOAT z/*ndc*/, D3D11_VIEWPORT vp, const DirectX::XMFLOAT4X4& view_projection)
{
	using namespace DirectX;
	XMFLOAT3 position;
	XMStoreFloat3(&position,
		XMVector3TransformCoord(
			XMVector3TransformCoord(
				XMVectorSet(static_cast<float>(x), static_cast<float>(y), z, 1),
				XMMatrixInverse(NULL,
					XMMatrixSet(
						vp.Width * 0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, -vp.Height * 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, vp.MaxDepth - vp.MinDepth, 0.0f,
						vp.TopLeftX + vp.Width * 0.5f, vp.Height * 0.5f + vp.TopLeftY, vp.MinDepth, 1.0f))
			), XMMatrixInverse(NULL, XMLoadFloat4x4(&view_projection))
		)
	);
	return position;
}
//playerの実装ができたらけす

void SceneGame::update(float elapsed_time)

{
	UIManager& ince = UIManager::incetance();
	
	if (ClearScreen(elapsed_time))
	{
		Menu_ = false;
		ClearRenderUiFlag = true;
		return;
	}
	//else ClearRenderUiFlag = false;
	if (Menu(elapsed_time))
	{
		MenuRenderUiFlag = true;
		return;
	}
	else MenuRenderUiFlag = false;
	//static float aaa = 0;

	//aaa+=elapsed_time;
	//if (aaa > 3)
	//{
	//	aaa = 0;
	//	SceneManagement::instance().SceneChange(new SceneLoading(new SceneTitle));
	//	
	//}

	gamepad& pad = gamepad::Instance();
	VMCFHT ince_ray = VMCFHT::instance();
	Objectmanajer& ince_o = Objectmanajer::incetance();
	
	//DropBox_Road* obj = Debug_ParameterObj.get();
	DirectX::XMFLOAT3 moveVec = {};
	//1人称移動
	{
		Camera& camera = Camera::instance();

		DirectX::XMFLOAT3 front = camera.GetFront();

		//スティックの動き取得
		float thumb_rx = pad.thumb_state_rx();
		float thumb_ry = pad.thumb_state_ry();


		moveVec.x = front.x * thumb_ry;
		moveVec.y = 0.0f;
		moveVec.z = front.z * thumb_ry;


		//前方向正規化
		float frontLength = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
		if (frontLength > 0.0f)
		{
			moveVec.x /= frontLength;
			moveVec.z /= frontLength;
		}
		XMFLOAT3 vec = moveVec;

		if (thumb_ry != 0.0f)
		{
			moveVec.x *= moveSpeed * elapsed_time;
			moveVec.z *= moveSpeed * elapsed_time;
			camera_position.x += moveVec.x;
			camera_position.z += moveVec.z;
		}


		DirectX::XMFLOAT3 right = camera.GetRight();

		moveVec.x = right.x * thumb_rx;
		moveVec.y = 0.0f;
		moveVec.z = right.z * thumb_rx;

		//前方向正規化
		frontLength = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
		if (frontLength > 0.0f)
		{
			moveVec.x /= frontLength;
			moveVec.z /= frontLength;
		}
		vec = moveVec;

		if (thumb_rx != 0.0f)
		{
			moveVec.x *= moveSpeed * elapsed_time;
			moveVec.z *= moveSpeed * elapsed_time;
			camera_position.x += moveVec.x;
			camera_position.z += moveVec.z;
			
		}
	}

	


	//プレイヤー更新処理
	/*PlayerManager::Instance().Update(elapsed_time);
	Player* player = PlayerManager::Instance().GetPlayer(0);
	*/

#ifdef _DEBUG
#if USE_IMGUI
	ImGui::Begin("SceneGameUpdate");
	ImGui::InputFloat3("camera_angle", &camera_angle.x);
	ImGui::InputFloat3("movevec", &moveVec.x);
	ImGui::End();
#endif
#endif
	plm.Update(elapsed_time);
	StageManager::incetance().Update(elapsed_time);
	if (Debug_ParameterObj)
	{
		Debug_ParameterObj->Update(elapsed_time);
	}
	Objectmanajer::incetance().Update(elapsed_time);


	//カメラ操作
	{
		currentCursorPos = SceneManagement::instance().GetCurrentCursorPosition();
		cursorPos = SceneManagement::instance().GetCursorPosition();

		camera_angle.y += (currentCursorPos.x - cursorPos.x) * sensi;
		camera_angle.x += (currentCursorPos.y - cursorPos.y) * sensi;

		/*camera_angle.y += (currentCursorPos.x - cursorPos.x) * sensi;
		camera_angle.x += (currentCursorPos.y - cursorPos.y) * -sensi;*/

		Player* pl = PlayerManager::Instance().GetPlayer(0);
		DirectX::XMFLOAT3 target = pl->GetPosition();
		if (GetKeyState('3') & 0x01)
		{
			target.y += 0.05f;
		}
		else
		{
			target.y += 0.5f;
		}
		camera_controller->SetTarget(target);
		if (mouseMove)
			camera_controller->SetAngle(camera_angle);

		float minAngle = XMConvertToDegrees(camera_controller->GetMinAngle());
		float maxAngle = XMConvertToDegrees(camera_controller->GetMaxAngle());
		if (camera_angle.x < minAngle)camera_angle.x = minAngle;
		if (camera_angle.x > maxAngle)camera_angle.x = maxAngle;

		DirectX::XMFLOAT3 Sangle;		//カメラの回転角保存
		//保存した回転角を注射器角度に入れるため一旦ラジアンから角度に変換
		Sangle.x = 0;
		Sangle.y = DirectX::XMConvertToRadians(180 + camera_angle.y);
		Sangle.z = 0;
		//保存した回転角をプレイヤー角度に入れるため一旦ラジアンから角度に変換
		DirectX::XMFLOAT3 plangle = pl->GetAngle();
		plangle.x = 0;
		plangle.y = DirectX::XMConvertToRadians(camera_angle.y);
		plangle.z = 0;
		//注射器の角度に入れる
		pl->SetSAngle(Sangle);
		pl->SetAngle(plangle);
		camera_controller->Update(elapsed_time);

	}
	//Goal_navi->Update(elapsed_time);
	//マウスカーソル操作変更
	if (GetAsyncKeyState('4') & 0x8000)
	{
		mouseMove = !mouseMove;
	}

	//マウスカーソル固定
	{
		Graphics& graphics = Graphics::Instance();
		float x, y;
		x = static_cast<float>(graphics.GetWindowSize().cx);
		y = static_cast<float>(graphics.GetWindowSize().cy);
		DirectX::XMFLOAT2 setCursorWindow =
		{
			SceneManagement::instance().GetWindowPosition().x + (x / 2),
			SceneManagement::instance().GetWindowPosition().y + (y / 2)
		};

		if (mouseMove) SetCursorPos(
			static_cast<int>(setCursorWindow.x),
			static_cast<int>(setCursorWindow.y)
		);
	}
	

}

void SceneGame::render(float elapsed_time)
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

	DirectX::XMMATRIX VP = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&rc.view), DirectX::XMLoadFloat4x4(&rc.projection));
	DirectX::XMStoreFloat4x4(&scene_data->data.view_projection, VP);

	Camera& camera = Camera::instance();
	rc.lightDirection.x = light_direction.x;
	rc.lightDirection.y = light_direction.y;
	rc.lightDirection.z = light_direction.z;
	rc.lightDirection.w = 0.0f;

	scene_data->data.light_direction.x = rc.lightDirection.x;
	scene_data->data.light_direction.y = rc.lightDirection.y;
	scene_data->data.light_direction.z = rc.lightDirection.z;

	scene_data->data.light_direction.w = 0.0f;

	scene_data->data.camera_position.x = camera.GetEye().x;
	scene_data->data.camera_position.y = camera.GetEye().y;
	scene_data->data.camera_position.z = camera.GetEye().z;
	scene_data->data.camera_position.w = 1.0f;

	DirectX::XMStoreFloat4x4(&scene_data->data.view_projection, VP);

	FLOAT color[] = {CLEAR_COLOR};
	framebuffers[0]->clear(graphics.GetDeviceContext(), color);
	framebuffers[0]->activate(graphics.GetDeviceContext());
	scene_data->activate(graphics.GetDeviceContext(), 1);
	parametric_constant->activate(graphics.GetDeviceContext(), 2);
	
	graphics.GetDeviceContext()->OMSetDepthStencilState(graphics.GetDepthStencilState(0), 0);
	graphics.GetDeviceContext()->RSSetState(graphics.GetRasterizerState(2));

	UIManager& ince_ui = UIManager::incetance();
	
	{

		StageManager::incetance().Render(&rc);
		if (Debug_ParameterObj)
		{
			Debug_ParameterObj->Render(&rc);
		}
		//オブジェクト描画処理
		//Goal_navi->Render(&rc);
		Objectmanajer::incetance().render(&rc);
		
		//プレイヤー描画処理
		plm.Render(&rc);

		plm.drawDrawPrimitive(graphics.GetDevice());
		

	}

	//imgui関連
	{
#ifdef _DEBUG
		ImGui::Begin("SceneGame");
		{
			/*	static std::string text="size1111111111111";
				std::string text1;
				int a=sizeof(text.c_str());
				ImGui::InputText("", const_cast<char*>(text.c_str()),10);
				text1 = text.c_str();
				ImGui::Text(const_cast<char*>(text1.c_str()));
				ImGui::InputInt("char_Size", &a);
				ImGui::InputFloat("Ray_dot", &d_dot_);

				XMFLOAT3 n_pos{ camera_position.x * 1.1f,camera_position.y,camera_position.z * 1.1f };*/

				/*
				ImGui::SliderFloat("camera_position.x*1.1", &n_pos.x, -100.0f, +100.0f);
				ImGui::SliderFloat("camera_position.y*1.1", &n_pos.y, -1.0f, +2.0f);
				ImGui::SliderFloat("camera_position.z*1.1", &n_pos.z, -100.0f, -1.0f);*/
		}
		DirectX::XMFLOAT3 c_pos{};
		DirectX::XMFLOAT3 p_pos{};

		if (ImGui::TreeNode("camera"))
		{
			ImGui::SliderFloat("input_camera_pos.x", &c_pos.x, -2.0f, +2.0f);
			ImGui::SliderFloat("input_camera_pos.y", &c_pos.y, -2.0f, +2.0f);
			ImGui::SliderFloat("input_camera_pos.z", &c_pos.z, -2.0f, +2.0f);
			ImGui::InputFloat("camera_pos.x", &camera_position.x, -2.0f, +2.0f);
			ImGui::InputFloat("camera_pos.y", &camera_position.y, -2.0f, +2.0f);
			ImGui::InputFloat("camera_pos.z", &camera_position.z, -2.0f, +2.0f);
			camera_position.x += c_pos.x;
			camera_position.y += c_pos.y;
			camera_position.z += c_pos.z;
			ImGui::TreePop();
		}
		if (Debug_ParameterObj)
		{

			Debug_ParameterObj->BaseGui();
		}
		if (ImGui::TreeNode("player"))
		{

			XMFLOAT3 front{ Camera::instance().GetFront() };
			XMFLOAT3 plpos = plm.GetPlayer(0)->GetPosition();
			ImGui::SliderFloat("input_pl.x", &p_pos.x, -1.0f, +1.0f);
			ImGui::SliderFloat("input_pl.y", &p_pos.y, -1.0f, +1.0f);
			ImGui::SliderFloat("input_pl.z", &p_pos.z, -1.0f, +1.0f);
			ImGui::InputFloat("plpos.x", &plpos.x, -1.0f, +1.0f);
			ImGui::InputFloat("plpos.y", &plpos.y, -2.0f, +2.0f);
			ImGui::InputFloat("plpos.z", &plpos.z, -1.0f, +1.0f);
			plpos.x += p_pos.x;
			plpos.y += p_pos.y;
			plpos.z += p_pos.z;
			plm.GetPlayer(0)->SetPosition(plpos);
			XMFLOAT3 vel = plm.GetPlayer(0)->GetVelocity();
			ImGui::InputFloat3("vel", &vel.x);
			ImGui::SliderFloat("set_vel", &vel.y, -100, 100);
			plm.GetPlayer(0)->SetVelocity(vel);
			ImGui::InputFloat("ms", &ms);
			ImGui::TreePop();
		}

		/*ImGui::SliderFloat("camera_front.x", &front.x, -100.0f, +100.0f);
		ImGui::SliderFloat("camera_front.y", &front.y, -1.0f, +2.0f);
		ImGui::SliderFloat("camera_front.z", &front.z, -100.0f, -1.0f);
		*/
		ImGui::SliderFloat("light_direction.x", &light_direction.x, -10.0f, +10.0f);
		ImGui::SliderFloat("light_direction.y", &light_direction.y, -10.0f, +10.0f);
		ImGui::SliderFloat("light_direction.z", &light_direction.z, -10.0f, +10.0f);

		
		auto p = [](ObjType type) {


			switch (type)
			{
			case ObjType::cution:
				return "cushion";
				break;
			case ObjType::Super_cution:
				return "Super_cution";
				break;
			case ObjType::Hard_to_Break:
				return "Hard_to_Break";
				break;
			case ObjType::Super_hard_to_Break:
				return "Super_hard_to_Break";
				break;
			case ObjType::heavy:
				return "heavy";
				break;
			case ObjType::Super_heavy:
				return "Super_heavy";
				break;
			case ObjType::Fragile:
				return "Fragile";
				break;
			case ObjType::Super_fragile:
				return "Super_fragile";
				break;
			case ObjType::null:
				return "null";
				break;
			default:
				break;
			}
			};


		if (Debug_ParameterObj != nullptr)
		{

		}

		ince_ui.Gui();

		ImGui::End();
		StageManager::incetance().Gui(graphics.GetDevice(), &rc);
		//Objectmanajer::incetance().Gui(graphics.GetDevice());
		//PlayerManager::Instance().DrawDebugGui();
		//GetAsyncKeyState(VK_LBUTTON);
		DebugRenderer& ince = DebugRenderer::incetance(graphics.GetDevice());
		ince.Render(graphics.GetDeviceContext(), rc.view, rc.projection);
#endif // !DEBUG
	}
	
	scene_data->deactivate(graphics.GetDeviceContext());
	
	parametric_constant->deactivate(graphics.GetDeviceContext());
	framebuffers[0]->deactivate(graphics.GetDeviceContext());

	graphics.GetDeviceContext()->OMSetDepthStencilState(graphics.GetDepthStencilState(3), 0);
	graphics.GetDeviceContext()->RSSetState(graphics.GetRasterizerState(2));

#if 1
	ID3D11ShaderResourceView* srvs[]
	{
		framebuffers[0]->shader_resource_views[0].Get(),	//color map
		framebuffers[0]->shader_resource_views[1].Get()		//depth map
	};

	graphics.GetBitBlockTransfer()->blit(graphics.GetDeviceContext(), srvs, 0, _countof(srvs), graphics.GetPixelShader(0));
#else
	graphics.GetBitBlockTransfer()->blit(graphics.GetDeviceContext(), framebuffers[0]->shader_resource_views[0].GetAddressOf(), 0, 1);
#endif
	
	//UI描画
	{
		ObjType type = PlayerManager::Instance().GetPlayer(0)->Getattribute();
		auto id = [](ObjType type)
			{
				string str = "";
				switch (type)
				{
				case ObjType::cution:
					str = "cution";
					return str;
					break;
				case ObjType::Super_cution:
					str = "Super_cution";
					return str;
					break;
				case ObjType::heavy:
					str = "heavy";
					return str;
					break;
				case ObjType::Super_heavy:
					str = "Super_heavy";
					return str;
					break;
				case ObjType::null:
					str = "null";
					return str;
					break;
				}
				return str;
			};
		ince_ui.Render(&rc, UI_StringID::CanbasID::Player, id(type));
		ince_ui.Render(&rc, UI_StringID::CanbasID::SceneGameUI);
		if (ClearRenderUiFlag)
		{
			ince_ui.Render(&rc, UI_StringID::CanbasID::GameClear);
		}
		else if (MenuRenderUiFlag)
		{
			ince_ui.Render(&rc, UI_StringID::CanbasID::Menu);
		}
	}

	
}

void SceneGame::finalize()
{

	StageManager::incetance().Clear();
	Objectmanajer::incetance().Clear();
	UIManager::incetance().Clear();
	Debug_ParameterObj = nullptr;
	//プレイヤー終了化
	PlayerManager::Instance().Clear();
}

void SceneGame::setFramebuffer()
{
	Graphics& graphics = Graphics::Instance();
	uint32_t width = static_cast<uint32_t>(graphics.GetWindowSize().cx);
	uint32_t height = static_cast<uint32_t>(graphics.GetWindowSize().cy);
	framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
	framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), width / 2, height / 2, DXGI_FORMAT_R16G16B16A16_FLOAT, true);
}

bool SceneGame::ClearScreen(float elapsedTime)
{
	Gimic*gimic = Objectmanajer::incetance().Select_GetGimic(Gimic_Type::Goal);
	SHORT keyState = GetAsyncKeyState(VK_LBUTTON);
	bool isKKeyPressed = (keyState & 0x8000) != 0;
	if (gimic->Get_GoalFlag())
	{
		UIManager& ince = UIManager::incetance();


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
					if (ui->GetID() == UI_StringID::UI_ID::Clear_Id::ClearNextStage)
					{

						if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetTexture2DDesc()))
						{
							ui->SetIsMouse(true);
							if (isKKeyPressed && !wasKeyPressed_mouse) {

								StageManager& ince_st = StageManager::incetance();
								for (StageName i = ince_st.GetStageName(); i != StageName::null;
									i = static_cast<StageName>(static_cast<int>(i) + 1))
								{
									if (ince_st.GetStageName() != i && i > ince_st.GetStageName())
									{
										ince_st.SetStageName(i);
										break;
									}
									if (ince_st.GetStageName() == StageName::stage1_3)
									{
										
									}
								}
								
							}
						}
					}
					if (ui->GetID() == UI_StringID::UI_ID::Clear_Id::ClearStageSelect)
					{

						if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetTexture2DDesc()))
						{
							ui->SetIsMouse(true);

							if (isKKeyPressed && !wasKeyPressed_mouse)
							{
								SceneManagement::instance().SceneChange(
									new SceneLoading(new Scene_Stage_Serect));
								break;
							}

						}
					}
					if (ui->GetID() == UI_StringID::UI_ID::Clear_Id::ClearTitle)
					{

						if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetTexture2DDesc()))
						{
							ui->SetIsMouse(true);

							if (isKKeyPressed && !wasKeyPressed_mouse)
							{
								SceneManagement::instance().SceneChange(
									new SceneLoading(new SceneTitle));
								break;
							}

						}
					}
				}
			}
		}
		ince.Update(elapsedTime);
		return true;
	}
	wasKeyPressed_mouse = isKKeyPressed;//今回キーが押されたかどうかを次回で使うために入れておく
	return false;
}

bool SceneGame::Menu(float elapsedTime)
{
	if (GetAsyncKeyState('K') & 0x8000) // 'K'キーが押されたかどうかを確認
	{
		if (!wasKeyPressed) // 前回のフレームでkが押されていない場合
		{
			Menu_ = !Menu_;
		}
		wasKeyPressed = true; // wasKeyPressedをtrueに設定
	}
	else
	{
		wasKeyPressed = false; // キーが押されていない場合はwasKeyPressedをfalseに設定
	}
	UIManager& ince = UIManager::incetance();
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
			if (ui->GetHanteiFlag()&&Menu_)
			{
				if (ui->GetID() == UI_StringID::UI_ID::Menu_Id::MenuContinue)
				{

					if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetScale()))
					{
						ui->SetIsMouse(true);
						if (isKKeyPressed && !wasKeyPressedMenu) {
							Menu_ = false;
						}
					}
				}
				if (ui->GetID() == UI_StringID::UI_ID::Menu_Id::MenuRitrai)
				{

					if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetScale()))
					{
						ui->SetIsMouse(true);
						if (isKKeyPressed && !wasKeyPressedMenu) {

							StageManager::incetance().SetStageName(StageManager::incetance().GetStageName());
							SceneManagement::instance().SceneChange(new SceneLoading(new SceneGame));
						}
					}
				}
				if (ui->GetID() == UI_StringID::UI_ID::Menu_Id::MenuGibuUp)
				{

					if (ince.Mouse_VS_UI(ui->GetPosition(), ui->GetScale()))
					{
						ui->SetIsMouse(true);
						if (isKKeyPressed && !wasKeyPressedMenu) {

							SceneManagement::instance().SceneChange(new SceneLoading(new SceneTitle));
						}
					}
				}
			
			}
		}

	}
	wasKeyPressedMenu = isKKeyPressed;//今回キーが押されたかどうかを次回で使うために入れておく
	ince.Update(elapsedTime);

	return Menu_;
}


