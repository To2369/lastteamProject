#include "camera_controller.h"
#include "Camera.h"
#include "Input/gamepad.h"

//�X�V����
void CameraController::Update(float elapsedTime)
{
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	
	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];//�s���3�s�ڂ̃f�[�^�����o���Ă���
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�ʒu����ǂ̕����Ƀ^�[�Q�b�g�����邩
	DirectX::XMFLOAT3 target;
	target.x = eye.x - front.x;
	target.y = eye.y - front.y;
	target.z = eye.z - front.z;

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}