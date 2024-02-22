#include "scene_title.h"
#include "camera.h"
#include "scene_game.h"
#include "scene_loading.h"
#include "Input/gamepad.h"
void SceneTitle::initialize(ID3D11Device* device,float x,float y)
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
	if (GetAsyncKeyState(VK_LBUTTON))
	{
	}
}

void SceneTitle::update(float elapsed_time, ID3D11Device* device, float x,float y)
{
	gamepad& pad = gamepad::Instance();
	camera_controller->AddAngle({ pad.thumb_state_ry() * 2.0f * elapsed_time,-pad.thumb_state_rx() * 2.0f * elapsed_time,0 });
	camera_controller->SetAngle({ 0.0f,-0.0f,0.0f });
	camera_controller->Update(elapsed_time);
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		SceneManagement::instance().SceneChange(new SceneLoading( new SceneGame));
	}
#if USE_IMGUI
	ImGui::Begin("sceneTitle");
	ImGui::End();
#endif
}

void SceneTitle::render(float elapsed_time, RenderContext& rc)
{
	rc.view = Camera::instance().GetView();
	rc.projection = Camera::instance().GetProjection();
	rc.lightDirection = light_direction;

	DirectX::XMMATRIX VP = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&rc.view), DirectX::XMLoadFloat4x4(&rc.projection));
	DirectX::XMStoreFloat4x4(&scene_data->data.view_projection, VP);

	scene_data->data.light_direction = rc.lightDirection;
	scene_data->data.camera_position.x = Camera::instance().GetFront().x;
	scene_data->data.camera_position.y = Camera::instance().GetFront().y;
	scene_data->data.camera_position.z = Camera::instance().GetFront().z;
	scene_data->data.camera_position.w = 1.0f;

	DirectX::XMStoreFloat4x4(&scene_data->data.view_projection, VP);
	scene_data->activate(rc.deviceContext, 1);
	parametric_constant->activate(rc.deviceContext, 2);

	//オブジェクト描画
	{

	}
}

void SceneTitle::finalize()
{

}
