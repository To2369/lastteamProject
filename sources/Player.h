#pragma once
#include"Character.h"
#include"Graphics/RenderContext.h"
#include"model.h"
//�v���C���[
class Player : public Character
{
public:
    Player(ID3D11Device* device);
    ~Player() override;

    //�X�V����
    void update(float elapsedTime);

    //�`�揈��
    void render(RenderContext* rc);

    Model* GetModel() { return model.get(); }

private:
    //����ړ�
    void inputMove(float elapsedTime);

    //���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 getMoveVec() const;
public:

    //�W�����v���͏���
    void inputJump();

    //�v���C���[�ƃM�~�b�N�̓����蔻��
    void CollisionPlayerVsGimics(float elapsedTime);

    void ExtractionAttribute(float elapsedTime);
protected:
    //���n�����Ƃ��ɌĂяo�����
    void OnLanding()override;
private:
    std::unique_ptr<Model> model;
    const char* filename = ".\\Resources\\Character\\Hand\\hand.fbx";

    //�ړ��X�s�[�h
    float moveSpeed = 1.0f;

    //����X�s�[�h
    float turnSpeed = DirectX::XMConvertToRadians(720);

    //�W�����v��
    float jumpSpeed = 1.0f;

    //�W�����v��
    int jumpCount = 0;

    //�W�����v����
    int jumpLimit = 2;

    //�v���C���[�̎�o���t���O
    bool isHand = true;
};
