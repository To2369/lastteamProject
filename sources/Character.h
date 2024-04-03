#pragma once
#include"model.h"
#include"DirectXMath.h"
//�L�����N�^�[
class Character
{
public:
	Character() {};
	virtual ~Character() {};
public://Set,Get,flag�֐�
    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition()const { return position; }

    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //��]�擾
    const DirectX::XMFLOAT3& GetAngle()const { return angle; }

    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //�X�P�[���ݒ�
    const DirectX::XMFLOAT3& GetScale()const { return scale; }

    //�X�P�[���擾
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //�n�ʂɐڂ��Ă��邩
    bool IsGround() const { return isGround; }
public:
    //�s��X�V����
    void UpdateTransform();
private:
    //�������͍X�V����
    void UpdateVerticalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);

    //�������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);
protected:
    //�ړ�����
    void Move(float vx, float vz, float speed);

    //���񏈗�
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //�W�����v����
    void Jump(float speed);

    //���͏����X�V
    void UpdateVelocity(float elapsedTime);

    //���n�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {}

protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };             //�ʒu
    DirectX::XMFLOAT3 angle = { 0,0,0 };                //�����p�x
    DirectX::XMFLOAT3 scale = { 1,1,1 };                //�傫��
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };                                                  //�`�̕ό`(���ׂ���ϐg�I�ȈӖ��炵����)
    DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };              //�F
    std::unique_ptr<Model> model;                       //���f��
protected:
    float gravity = -1.0f;                              //�d��
    DirectX::XMFLOAT3 velocity = { 0,0,0 };             //���x

    bool isGround = false;                              //�n�ʂƐڒn���Ă��邩
    
    float friction = 0.5f;                              //���C
    float acceleration = 1.0f;                          //�����x
    float maxMoveSpeed = 5.0f;                          //�ő呬�x

    float moveVecX = 0.0f;                              //�v�Z�p
    float moveVecZ = 0.0f;                              //�v�Z�p

    float airControl = 0.3f;                            //�󒆐���(�ړ�)
    
    float stepOffset = 1.0f;                            //�J�n�ʒu����ɂ��邽��

    float slopeRate = 1.0f;                              //�X�Η�
};