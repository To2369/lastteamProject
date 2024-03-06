#include "variable_management_class_for_hit_test.h"
#include "scene_game.h"
#include "Input/gamepad.h"
#include "camera.h"


#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

int test = 0;
void SceneGame::initialize(ID3D11Device* device, float x, float y)
{
	device_ = device;
	Camera& camera = Camera::instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(90),
		x / y,
		0.1f,
		1000.0f
	);
	camera_controller = std::make_unique<CameraController>();

	//定数バッファ生成
	{
		scene_data = std::make_unique<constant_buffer<scene_constants>>(device);
		parametric_constant = std::make_unique<constant_buffer<parametric_constants>>(device);
	}
	GetAsyncKeyState(VK_RBUTTON);

	//プレイヤー初期設定
	PlayerManager& playerManager = PlayerManager::Instance();
	unique_ptr<Player> player = make_unique<Player>(device);
	player->SetPosition({ -5,0,0 });
	PlayerManager::Instance().Register(std::move(player));
	{
		StageManager& ince = StageManager::incetance();
		ince.Initialize_GameStage(StageName::stage1_1, device);
	}
	//Debug_ParameterObj = make_unique<Switch>(device);
	//Debug_ParameterObj->SetPosition({1.f, 0.3f, 0.5f});

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



void SceneGame::update(float elapsed_time, ID3D11Device* device, float x,float y)

{
	gamepad& pad = gamepad::Instance();
	
	//1人称移動
	{
		Camera& camera = Camera::instance();

		DirectX::XMFLOAT3 front = camera.GetFront();

		//スティックの動き取得
		float thumb_rx = pad.thumb_state_rx();
		float thumb_ry = pad.thumb_state_ry();

		DirectX::XMFLOAT3 moveVec = {};
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

		if (thumb_ry != 0.0f)
		{
			moveVec.x *= moveSpeed*elapsed_time;
			moveVec.z *= moveSpeed*elapsed_time;

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

		if (thumb_rx != 0.0f)
		{
			moveVec.x *= moveSpeed*elapsed_time;
			moveVec.z *= moveSpeed*elapsed_time;

			camera_position.x += moveVec.x;
			camera_position.z += moveVec.z;
			

		}
	}

	//プレイヤー更新処理
	PlayerManager::Instance().Update(elapsed_time);
	Player* player = PlayerManager::Instance().GetPlayer(0);
#ifdef _DEBUG
#if USE_IMGUI
	ImGui::Begin("SceneGameUpdate");
	ImGui::InputFloat3("camera_angle", &camera_angle.x);
	ImGui::End();
#endif
#endif
	StageManager::incetance().Update(elapsed_time);
	Objectmanajer::incetance().Update(elapsed_time);
	//Debug_ParameterObj->Update(elapsed_time);
	//カメラ操作
	{
		currentCursorPos = SceneManagement::instance().GetCurrentCursorPosition();
		cursorPos = SceneManagement::instance().GetCursorPosition();

		//camera_angle.y += (setCursorWindow.x - cursorPos.x) * -sensi;
		//camera_angle.x += (setCursorWindow.y - cursorPos.y) * sensi;

		camera_angle.y += (currentCursorPos.x - cursorPos.x) * sensi;
		camera_angle.x += (currentCursorPos.y - cursorPos.y) * -sensi;

		camera_controller->SetEye(camera_position);
		if (mouseMove)
			camera_controller->SetAngle(camera_angle);

		float minAngle = XMConvertToDegrees(camera_controller->GetMinAngle());
		float maxAngle = XMConvertToDegrees(camera_controller->GetMaxAngle());
		if (camera_angle.x < minAngle)camera_angle.x = minAngle;
		if (camera_angle.x > maxAngle)camera_angle.x = maxAngle;

		camera_controller->Update(elapsed_time);

	}
	
	//マウスカーソル操作変更
	if (GetKeyState('4') & 1)
	{
		mouseMove = !mouseMove;
	}

	//マウスカーソル固定
	{
		DirectX::XMFLOAT2 setCursorWindow =
		{
			SceneManagement::instance().GetWindowPosition().x + (x / 2),
			SceneManagement::instance().GetWindowPosition().y + (y / 2)
		};

		if (mouseMove) SetCursorPos(
			setCursorWindow.x,
			setCursorWindow.y
		);
	}
	

}

void SceneGame::render(float elapsed_time,RenderContext& rc)
{
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
	scene_data->activate(rc.deviceContext, 1);
	parametric_constant->activate(rc.deviceContext, 2);
	
	{
		//プレイヤー描画処理
		//PlayerManager::Instance().Render(rc.deviceContext);
		
		
		//Debug_ParameterObj->Render(&rc);
		StageManager::incetance().Render(&rc);
		//オブジェクト描画処理
		Objectmanajer::incetance().render(&rc);
	}

	//imgui関連
	{
#ifdef _DEBUG
		ImGui::Begin("SceneGame");
	
		//bool f = Debug_ParameterObj->Get_GoalFlag();
		//ImGui::Checkbox("GoalFlag",&f);
	/*	XMFLOAT3 a = Debug_ParameterObj->GetPosition();
		ImGui::SliderFloat("position.x", &a.x, -0.0f, +10.0f);
		ImGui::SliderFloat("position.y", &a.y, -0.0f, +10.0f);
		ImGui::SliderFloat("position.z", &a.z, -0.0f, +10.0f);
		Debug_ParameterObj->SetPosition(a);*/
		

		ImGui::SliderFloat("camera_position.x", &camera_position.x, -100.0f, +100.0f);
		ImGui::SliderFloat("camera_position.y", &camera_position.y, -100.0f, +100.0f);
		ImGui::SliderFloat("camera_position.z", &camera_position.z, -100.0f, -1.0f);

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
		
		
		if (Debug_ParameterPlayer != nullptr)
		{
			const int ii = 2;
			for (int i = 0; i < ii; i++)
			{
				ImGui::Text("Object_Type:%s", p(Debug_ParameterPlayer->Get_Objtype(i)));
				float t = Debug_ParameterPlayer->GetReturnTimer(i);
				ImGui::InputFloat("return_timer", &t);

			}
		}



		ImGui::End();
		StageManager::incetance().Gui(device_.Get(),&rc);
		Objectmanajer::incetance().Gui(device_.Get());
		//GetAsyncKeyState(VK_LBUTTON);
		GetAsyncKeyState(VK_RBUTTON);
#endif // !DEBUG
	}

	//UI描画
	{
	}
}

void SceneGame::finalize()
{

	StageManager::incetance().Clear();
	Objectmanajer::incetance().Clear();
	//プレイヤー終了化
	PlayerManager::Instance().Clear();
	
}


