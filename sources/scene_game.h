#pragma once

#include "scene_management.h"
#include "Graphics/RenderContext.h"
#include "camera_controller.h"
#include "Graphics/sprite.h"
#include"Heavy.h"
#include"Cution.h"
#include "objectManajer.h"
#include"PlayerManager.h"
#include"Graphics/sprite.h"
#include"Stage.h"
#include "StageManager.h"
#include"Goal_navi.h"
#include "Graphics/sky_map.h"
#include"Tutorial.h"
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame()
	{
		
	}

	//開始処理
	void initialize()override;
	//更新処理
	void update(float elapsed_time)override;
	//描画処理
	void render(float elapsed_time)override;
	//終了処理
	void finalize()override;

	//framebuffer設定
	void setFramebuffer()override;

	//void DebugMode_MouseRayCast(DebugMode mode,ID3D11Device* device);
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
	};
	std::unique_ptr<constant_buffer<scene_constants>> scene_data;

private:
	std::unique_ptr<CameraController> camera_controller = nullptr;
	DirectX::XMFLOAT4 light_direction{ 0.0f,-0.5f,0.5f,0.0f };

	DirectX::XMFLOAT3 camera_angle = { 0,0,0 };
	DirectX::XMFLOAT2 cursorPos = {};
	DirectX::XMFLOAT2 currentCursorPos = {};
	float sensi = 0.1f;
	bool mouseMove = true;

	DirectX::XMFLOAT3 camera_position{ 0.0f,24.0f, 0.0f };
	std::unique_ptr<sprite>MaskSprite;

	std::unique_ptr<sky_map> sky = nullptr;
private:
	//std::unique_ptr<Object>game_obj;
	Intersection result_intersection{};
	//unique_ptr<DropBox_Road> Debug_ParameterObj=nullptr;
	std::unique_ptr<Stage>Debug_ParameterObj=nullptr;
	//Object* Debug_ParameterPlayer = nullptr;
	unique_ptr<collision_mesh> debug_collition;
	unique_ptr<Goal_navigation_Arrow>Goal_navi;
	unique_ptr<sprite> text;
	float moveSpeed = 1.5f;
	float ms = 0;
	bool Menu_ = false;
	bool MenuRenderUiFlag = false;
	bool ClearRenderUiFlag = false;
	bool wasKeyPressed = false;
	bool wasKeyPressed_mouse = false;
	bool wasKeyPressedMenu = false;
	PlayerManager& plm = PlayerManager::Instance();
	bool ClearScreen(float elapsedTime);
	bool Menu(float elapsedTime);
	std::unique_ptr<Tutorial> tutorial=nullptr;
	//マウスの表示
	bool disPlayCorsor = false;		
};