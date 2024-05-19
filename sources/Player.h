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
    void updateSyringepos();

    //�W�����v���͏���
    void inputJump();

    //�v���C���[�ƃM�~�b�N�̓����蔻��
    void CollisionPlayerVsGimics(float elapsedTime);

    void ExtractionAttribute(float elapsedTime);

    void pullpushAnime(float elapsedTime);
protected:
    //���n�����Ƃ��ɌĂяo�����
    void OnLanding()override;
private:
    std::unique_ptr<Model> model;
    std::unique_ptr<Model> Smodel;          //���ˊ�p
    const char* filename = ".\\Resources\\Character\\Hand\\hand.fbx";
    const char* Sfilename = ".\\Resources\\Character\\Syringe\\syringe_M.fbx";      //���ˊ�p

    DirectX::XMFLOAT3 resetPosition = {};       //�X�e�[�W���痎�������ۂɏ����ʒu�ɖ߂�
    //�ړ��X�s�[�h
    float moveSpeed = 1.0f;

    //����X�s�[�h
    float turnSpeed = DirectX::XMConvertToRadians(720);

    //�W�����v��
    float jumpSpeed = 0.8f;

    //�W�����v��
    int jumpCount = 0;

    //�W�����v����
    int jumpLimit = 1;

    //�v���C���[�̎�o���t���O
    bool isHand = true;

    float elapsedTime_ = 0;

    bool wasKeyPressed = false;

    bool pullType = false;
    bool pushType = false;
    
    bool pushFlag = false;  //��������Ƃ�

    bool SphereHitFlag = false;
    bool CubeHitFlag = false;

    enum Sanime
    {
        wait,
        pull,
        push,
    };
};
