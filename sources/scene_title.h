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
	//�J�n����
	void initialize()override;
	//�X�V����
	void update(float elapsed_time)override;
	//�`�揈��
	void render(float elapsed_time)override;
	//�I������
	void finalize()override;
	//framebuffer�ݒ�
	void setFramebuffer() override;
private:
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
		DirectX::XMFLOAT4X4 light_view_projection;  //���C�g�̈ʒu���猩���ˉe�s��
	};
	std::unique_ptr<constant_buffer<scene_constants>> scene_data;

	std::unique_ptr<sprite> title;
	std::unique_ptr<sprite> push_click;

	DirectX::XMFLOAT4 light_direction{ -0.2f,-0.353f,0.331f,0.0f };
	
	std::unique_ptr<CameraController> camera_controller = nullptr;

	float timer = 0.0f;
	bool wasKeyPressed = false;

};