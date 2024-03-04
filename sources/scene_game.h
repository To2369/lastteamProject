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
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() 
	{
		
	}

	//�J�n����
	void initialize(ID3D11Device* device,float x = 1280,float y = 720)override;
	//�X�V����
	void update(float elapsed_time,ID3D11Device* device,float x,float y)override;
	//�`�揈��
	void render(float elapsed_time, RenderContext& rc)override;
	//�I������
	void finalize()override;
	 
	//void DebugMode_MouseRayCast(DebugMode mode,ID3D11Device* device);
	//�萔 
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
		DirectX::XMFLOAT4X4 view_projection;	//�r���[�E�v���W�F�N�V�����ϊ��s��
		DirectX::XMFLOAT4 light_direction;		//���C�g�̌���
		DirectX::XMFLOAT4 camera_position;		//�J�����ʒu
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
	
	DirectX::XMFLOAT3 camera_position{ 0.0f, 1.0f, 0.0f };
private:
	std::unique_ptr<Object>game_obj;
	Intersection result_intersection{};
	unique_ptr<Switch> Debug_ParameterObj=nullptr;
	Object* Debug_ParameterPlayer = nullptr;
	unique_ptr<collision_mesh> debug_collition;
	unique_ptr<sprite> text;
	float moveSpeed = 1.5f;
	Player* player = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
};