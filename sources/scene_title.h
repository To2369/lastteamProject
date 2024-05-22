#pragma once
#include "scene_management.h"
#include<vector>
#include "Graphics/sprite.h"
#include "model.h"
#include "camera_controller.h"
#include"UI.h"

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle()override;
	//開始処理
	void initialize()override;
	//更新処理
	void update(float elapsed_time)override;
	//描画処理
	void render(float elapsed_time)override;
	//終了処理
	void finalize()override;
	//framebuffer設定
	void setFramebuffer() override;
private:
	//定数
	struct parametric_constants
	{
		float extraction_threshold{ 0.8f };
		float gaussian_sigma{ 1.0f };
		float bloom_intensity{ 1.0f };
		float exposure{ 1.0f };
	};
	std::unique_ptr<constant_buffer<parametric_constants>> parametric_constant;

	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection;	//ビュー・プロジェクション変換行列
		DirectX::XMFLOAT4 light_direction;		//ライトの向き
		DirectX::XMFLOAT4 camera_position;		//カメラ位置
		DirectX::XMFLOAT4X4 light_view_projection;  //ライトの位置から見た射影行列
	};
	std::unique_ptr<constant_buffer<scene_constants>> scene_data;

	std::unique_ptr<sprite> title;
	std::unique_ptr<sprite> push_click;

	DirectX::XMFLOAT4 light_direction{ -0.2f,-0.353f,0.331f,0.0f };
	
	std::unique_ptr<CameraController> camera_controller = nullptr;

	float timer = 0.0f;
	bool wasKeyPressed = false;

};