#include "variable_management_class_for_hit_test.h"
#include "scene_game.h"
#include "Input/gamepad.h"
#include "camera.h"
#include "objectManajer.h"
#include"StageManager.h"

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

	Objectmanajer& obj_Manager = Objectmanajer::incetance();
	{
		unique_ptr<Metal> obj = make_unique<Metal>(device);
		obj->SetPosition({ 0.f, 0.5f, -1.0f });
		obj_Manager.Rigister_obj(std::move(obj));
		
	}
	{
		unique_ptr<DestroyObj> obj = make_unique<DestroyObj>(device);
		obj->SetPosition({ 0.f,0.500659713871f,-0.5f });
		obj_Manager.Rigister_obj(std::move(obj));
	
	}

	StageManager& stage_manager = StageManager::incetance();
	{
		unique_ptr<DebugStage>stage = make_unique<DebugStage>(device);
		stage->SetPosition({ 0.f, 0.5f, -0.5f });
		stage_manager.Rigister(move(stage));
	}
	Debug_ParameterObj = nullptr;
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
	
	{// COLLISION_MESH
		if (GetAsyncKeyState(VK_LBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data->data.view_projection, scene_data->data.camera_position);

			result_intersection = {};
			int gameobject_count = Objectmanajer::incetance().Get_GameObjCount();
			for (int i = 0; i < gameobject_count; i++)
			{
				Object* obj = Objectmanajer::incetance().Get_GameObject(i);

				if(VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(),obj->GetTransform(),result_intersection))
				{
					OutputDebugStringA("Intersected : ");
					OutputDebugStringA(result_intersection.intersection_mesh.c_str());
					OutputDebugStringA(" : ");
					OutputDebugStringA(result_intersection.intersection_material.c_str());
					OutputDebugStringA("\n");
					player->Set_attribute(obj->Get_attribute());
					Debug_ParameterObj = obj;
					Debug_ParameterPlayer = player;
					test = 1;
					break;
				}
				else
				{
					Debug_ParameterObj = nullptr;
					Debug_ParameterPlayer = nullptr;
					OutputDebugStringA("Unintersected...\n");
				}
			}
		}
	}
	Objectmanajer::incetance().Update(elapsed_time);
	if(GetAsyncKeyState(VK_RBUTTON) & 1)
	{
		VMCFHT::instance().update(scene_data->data.view_projection, scene_data->data.camera_position);
		result_intersection = {};
		int gameobject_count = Objectmanajer::incetance().Get_GameObjCount();
		for (int i = 0; i < gameobject_count; i++)
		{
			Object* obj = Objectmanajer::incetance().Get_GameObject(i);

			if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
			{
				obj->Set_attribute(player->Get_attribute());
				Debug_ParameterObj = obj;
				Debug_ParameterPlayer = player;
				test = 0;
				break;
			}
			else
			{
				Debug_ParameterPlayer = nullptr;
				Debug_ParameterObj = nullptr;
			}
		}
	}

	StageManager::incetance().Update(elapsed_time);
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
	if (GetKeyState('4') == 1)
	{
		mouseMove = !mouseMove;
	}

	//マウスカーソル固定
	{
		DirectX::XMFLOAT2 setCursorWindow = {
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

		//オブジェクト描画処理
		Objectmanajer::incetance().render(rc.deviceContext);
		StageManager::incetance().Render(rc.deviceContext);
	}

	//imgui関連
	{
#ifdef _DEBUG
		ImGui::Begin("SceneGame");
		ImGui::Text("intersected_position : %0.2f, %0.2f, %0.2f",
			result_intersection.intersection_position.x,
			result_intersection.intersection_position.y,
			result_intersection.intersection_position.z);
		ImGui::Text("intersection_normal : %0.2f, %0.2f, %0.2f",
			result_intersection.intersection_normal.x, result_intersection.intersection_normal.y, result_intersection.intersection_normal.z);
		ImGui::Text("intersected_mesh : %s", result_intersection.intersection_mesh.c_str());
		ImGui::Text("intersected_material : %s", result_intersection.intersection_material.c_str());

		ImGui::SliderFloat("camera_position.x", &camera_position.x, -100.0f, +100.0f);
		ImGui::SliderFloat("camera_position.y", &camera_position.y, -100.0f, +100.0f);
		ImGui::SliderFloat("camera_position.z", &camera_position.z, -100.0f, -1.0f);

		ImGui::SliderFloat("light_direction.x", &light_direction.x, -10.0f, +10.0f);
		ImGui::SliderFloat("light_direction.y", &light_direction.y, -10.0f, +10.0f);
		ImGui::SliderFloat("light_direction.z", &light_direction.z, -10.0f, +10.0f);

		//for (int i = 0; i < Objectmanajer::incetance().Get_GameObjCount(); i++)
		//{
		//	Objectmanajer& ince = Objectmanajer::incetance();
		//	static XMFLOAT3 pos = {};
		//	ImGui::SliderFloat3("objPos", &pos.x, -10.0f, -10.0f);
		//	ince.Get_GameObject(i)->SetPosition(pos);

		//}

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
			string name = p(Debug_ParameterObj->Get_attribute());
			ImGui::Text("Object_Type:%s", name.c_str());
			float t = Debug_ParameterObj->GetReturnTimer();
			ImGui::InputFloat("return_timer", &t);
		}
		if (Debug_ParameterPlayer != nullptr)
		{
			string name = p(Debug_ParameterPlayer->Get_attribute());
			ImGui::Text("Object_Type:%s", name.c_str());
			float t = Debug_ParameterPlayer->GetReturnTimer();
			ImGui::InputFloat("return_timer", &t);
		}



		ImGui::End();
#endif // !DEBUG
	}

	//UI描画
	{
	}
}

void SceneGame::finalize()
{
	if (Debug_ParameterObj != nullptr)
	{
		Debug_ParameterObj = nullptr;
		delete Debug_ParameterObj;
	}

	Objectmanajer::incetance().Clear();
	//プレイヤー終了化
	PlayerManager::Instance().Clear();
	StageManager::incetance().Clear();
}
