//#pragma once
//#include"object.h"
//#include"model.h"
//class Player:public Object
//{
//public:
//
//	Player(ID3D11Device* device);
//	~Player()override;
//
//	//�X�V����
//	void Update(float elapsedTime)override;
//
//	//�`�揈��
//	void Render(RenderContext* rc)override;
//
//private:
//    //�ҋ@�X�e�[�g�֑J��
//    void TransitionIdleState();
//
//    //�ҋ@�X�e�[�g�X�V����
//    void UpdateIdleState(float elapsedTime);
//
//    //�ړ��X�e�[�g�֑J��
//    void TransitionMoveState();
//
//    //�ړ��X�e�[�g�X�V����
//    void UpdateMoveState(float elapsedTime);
//
//    //�W�����v�X�e�[�g�֑J��
//    void TransitionJumpState();
//
//    //�W�����v�X�e�[�g�X�V����
//    void UpdateJumpState(float elapsedTime);
//
//    //���n�X�e�[�g�֑J��
//    void TransitionLandState();
//
//    //���n�X�e�[�g�X�V����
//    void UpdateLandState(float elapsedTime);
//private:
//    //�ړ����͏���
//    bool InputMove(float elapsedTime);
//
//    //�W�����v���͍X�V
//    bool InputJump();
//
//    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
//    DirectX::XMFLOAT3 GetMoveVec()const;
//
//    float turnSpeed = DirectX::XMConvertToRadians(720);
//
//public:
//    
//
//    //�������͍X�V����
//    void UpdateVerticalVelocity(float elapsedFrame);
//
//    //�����ړ��X�V����
//    void UpdateVerticalMove(float elapsedTime);
//
//    //�������͍X�V����
//    void UpdateHorizontalVelocity(float elapsedFrame);
//
//    //�����ړ��X�V����
//    void UpdateHorizontalMove(float elapsedTime);
//
//public:
//    //�ړ�����
//    void Move(float vx, float vz, float speed);
//
//    //���񏈗�
//    void Turn(float elapsedTime, float vx, float vz, float speed);
//
//    //�W�����v����
//    void Jump(float speed);
//
//    //���͏����X�V
//    void UpdateVelocity(float elapsedTime);
//
//    //���n�����Ƃ��ɌĂ΂��
//    virtual void OnLanding() {}
//private:
//    //�d��
//    float gravity = -1.0f;
//    //�n�ʂɒ��n������
//    bool isGround = false;
//    //���C
//    float friction = 0.5f;
//
//    //�����x
//    float acceleration = 1.0f;
//
//    //�ő呬�x
//    float maxMoveSpeed = 5.0f;
//
//    //�v�Z�p
//    float moveVecX = 0.0f;
//    float moveVecZ = 0.0f;
//
//    //�󒆈ړ�
//    float airControl = 0.3f;
//
//    //�J�n�ʒu����ɂ��邽��
//    float stepOffset = 1.0f;
//
//    //�΂߂̎��Ԑ���
//    float slopeRate = 1.0f;
//protected:
//    //���n�����Ƃ��ɂ�΂��
//    void OnLanding()override;
//private:
//	const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
//    enum class State
//    {
//        Idle,
//        Move,
//        Jump,
//        Land
//    };
//    State state = State::Idle;
//    float moveSpeed = 5.0f;         //�ړ����x
//    float jumpSpeed = 20.0f;        //�W�����v���x
//    int jumpCount = 0;              //�W�����v�̃J�E���g
//    int jumpLimit = 2;              //�W�����v�ł����
//};