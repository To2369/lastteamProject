#pragma once
#include"model.h"
#include"DirectXMath.h"
#include"object.h"
//�L�����N�^�[
class Character
{
public:
    Character() {}
    virtual ~Character() {}

    //�s��X�V����
    void UpdateTransform();

private:
    //�������x�X�V����
    void updateVerticalVelocity(float elapsedTime);

    //�����ړ��X�V����
    void updateVerticalMove(float elapsedTime);

    //�������x�X�V����
    void updateHorizontalVelocity(float elapsedSTIme);

    //�����ړ��X�V����
    void updateHorizontalMove(float elapsedTime);
protected:
    //�ړ�����
    void Move(float vx, float vz, float speed);

    //���񏈗�
    void turn(float elapsedTime, float vx, float vz, float speed);

    //�W�����v����
    void jump(float speed);

    //���x�����X�V
    void updateVelocity(float elapsedTime);

   // ���n�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {}
public:
    //�ʒu�擾
    const DirectX::XMFLOAT3 GetPosition()const { return position; };

    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; };

    //��]�擾
    const DirectX::XMFLOAT3 GetAngle()const { return angle; };

    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; };

    //��]�擾
    const DirectX::XMFLOAT3 GetSAngle()const { return Sangle; };

    //��]�ݒ�
    void SetSAngle(const DirectX::XMFLOAT3& Sangle) { this->Sangle = Sangle; };

    //�X�P�[���ݒ�
    const DirectX::XMFLOAT3 GetScale()const { return scale; };

    //�X�P�[���擾
    void  SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; };

    //���x�擾
    DirectX::XMFLOAT3 GetVelocity() { return velocity; };

    //���x�ݒ�
    void SetVelocity(XMFLOAT3 vel) { velocity = vel; }

    //���a�̎擾
    float getRadius()const { return radius; }

    //�F�̎擾
    DirectX::XMFLOAT4 GetColor() { return color; }

    //�F�̐ݒ�
    void SetColor(const DirectX::XMFLOAT4& color) { this->color; }

   // ���n���������ۂ̃t���O�擾
    bool isGrounded() const { return groundedFlag; }

    //�v���C���[���������Ă��鑮��
    Obj_attribute Getattribute() { return playerType; }
protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };         //�ʒu
    DirectX::XMFLOAT3 Sposition = { 0,0,0 };         //�ʒu(���ˊ�p)
    DirectX::XMFLOAT3 angle = { 0,0,0 };            //��]
    DirectX::XMFLOAT3 Sangle = { 0,0,0 };            //��](���ˊ�p)
    DirectX::XMFLOAT3 scale = { 1,1,1 };            //�g��k��
    DirectX::XMFLOAT3 Sscale = { 1,1,1 };            //�g��k��(���ˊ�p)
    DirectX::XMFLOAT4 color = { 1,1,1,1 };          //�F
    DirectX::XMFLOAT4 Scolor = { 1,1,1,1 };          //�F(���ˊ�p)
    //�p���s��
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    //�p���s��(���ˊ�p)
    DirectX::XMFLOAT4X4 Stransform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    float radius =0.f;                            //���a

    //�d��
    float gravity = -1.0f;

   // ���x
    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    bool groundedFlag = false;

    float friction = 0.5f;                          //���C��

    float acceleration = 1.0f;                      //�����x
    float maxMoveSpeed = 5.0f;                      //�ő呬�x
    DirectX::XMFLOAT3 direction = { 0,0,0 };        //�ړ�����
    float airControl = 0.0001f;                        //�󒆐���p�W��
    bool extractFlag = false;                       //�I�u�W�F�N�g�̑������o�ł��邩
    ObjType playerType = ObjType::null;
    //�΂߂̎��Ԑ���
    float slopeRate = 1.0f;
};