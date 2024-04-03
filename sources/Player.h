#pragma once
#include"Character.h"
#include"Graphics/RenderContext.h"
#include"model.h"
class Player:public Character
{
public:

	Player(ID3D11Device* device);
	~Player()override;

	//�X�V����
    void Update(float elapsedTime);

	//�`�揈��
	void Render(RenderContext* rc);

private:
    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ��X�e�[�g�֑J��
    void TransitionMoveState();

    //�ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTime);

    //�W�����v�X�e�[�g�֑J��
    void TransitionJumpState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJumpState(float elapsedTime);

    //���n�X�e�[�g�֑J��
    void TransitionLandState();

    //���n�X�e�[�g�X�V����
    void UpdateLandState(float elapsedTime);
private:
    //�ړ����͏���
    bool InputMove(float elapsedTime);

    //�W�����v���͍X�V
    bool InputJump();

    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec()const;

    float turnSpeed = DirectX::XMConvertToRadians(720);
protected:
    //���n�����Ƃ��ɂ�΂��
    void OnLanding()override;
private:
	const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land
    };
    State state = State::Idle;
    float moveSpeed = 5.0f;         //�ړ����x
    float jumpSpeed = 20.0f;        //�W�����v���x
    int jumpCount = 0;              //�W�����v�̃J�E���g
    int jumpLimit = 2;              //�W�����v�ł����
};