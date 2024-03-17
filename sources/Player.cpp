#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
Player::Player(ID3D11Device*device)
{
    model = make_unique<Model>(device, filename, true);

    //Player����������\�͂̏����ݒ�
    old_attribute_state.push_back(Obj_attribute::null);

    //���f���̑傫���ݒ�
    Scale.x = Scale.y = Scale.z = 100.0f;

    //�����̍s���ݒ�
    TransitionIdleState();
}

Player::~Player()
{

}

void Player::Update(float elapsedTime)
{
    //�X�e�[�g���̏���
    switch (state)
    {
    case State::Idle:UpdateIdleState(elapsedTime);
        break;
    case State::Move:UpdateMoveState(elapsedTime);
        break;
    case State::Jump:UpdateJumpState(elapsedTime);
        break;
    case State::Land:UpdateLandState(elapsedTime);
        break;
    }

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();
}

void Player::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform);
}

bool Player::InputMove(float elapsedTime)
{
    // �i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //�i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec()const
{
    //���͏����擾
    gamepad& pad = gamepad::Instance();
    float ax = pad.thumb_state_rx();
    float ay = pad.thumb_state_ry();
    //�J�����������擾
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    //�ړ��x�N�g����XZ���ʂȃx�N�g���ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }
    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength < 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
    //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f��
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f��
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;
    return vec;
}

//�W�����v���͏���
bool Player::InputJump()
{
    gamepad& pad = gamepad::Instance();
    if (GetKeyState(' ') & 1)//�R���g���[���[��A�ɂ������������킩��Ȃ�
    {
        //�W�����v�񐔐���
        if (jumpCount < jumpLimit)
        {
            //�W�����v
            jumpCount++;
            Jump(jumpSpeed);
            //�W�����v���͂���
            return true;
        }
    }
    return false;
}

void Player::OnLanding()
{
    jumpCount = 0;
    if (Velocty.y >= -1)
    {
        TransitionLandState();
    }
    //�������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
    if (Velocty.y < gravity * 5.0f)
    {
        //���n�X�e�[�g�֑J��
        TransitionLandState();
    }
}

//�ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
    state = State::Idle;

    //�ҋ@�A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Idle, true);
}

//�ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(float elapsedTime)
{
    //�ړ����͏���
    InputMove(elapsedTime);
    if (InputMove(elapsedTime) == true)
    {
        //�ړ��X�e�[�g�ւ̑J��
        TransitionMoveState();
    }

    //�W�����v���͏���
    InputJump();
    if (InputJump() == true)
    {
        //�W�����v�X�e�[�g�ւ̑J��
        TransitionJumpState();
    }
}

//�ړ��X�e�[�g�֑J��
void Player::TransitionMoveState()
{
    state = State::Move;

    //����A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Running, true);
}

//�ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{
    //�ړ����͏���
    if (InputMove(elapsedTime) == false)
    {
        TransitionIdleState();
    }

    //�W�����v���͏���
    if (InputJump() == true)
    {
        //�W�����v�X�e�[�g�ւ̑J��
        TransitionJumpState();
    }
}

//�W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
    state = State::Jump;

    //�W�����v�A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_jump, false);
}

//�W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(float elapsedTime)
{
    //�ړ����͏���
    InputMove(elapsedTime);

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�t���b�v�A�j���[�V�����Đ�
        //model->PlayAnimation(Anim_jump_Flip, false);
    }
    //�W�����v�A�j���[�V�����I����
    //if (!model->IsPlayAnimation())
    //{
    //    //�����A�j���[�V�����Đ�
    //    model->PlayAnimation(Anim_Falling, true);
    //}
}

//���n�X�e�[�g�֑J��
void Player::TransitionLandState()
{
    state = State::Land;

    //���n�A�j���[�V�������Đ�
    //model->PlayAnimation(Anim_Landing, false);
}

//���n�X�e�[�g�X�V����
void Player::UpdateLandState(float elapsedTime)
{
    //�U���A�j���[�V�����I����
    //if (!model->IsPlayAnimation())
    //{
    //    //�ҋ@�X�e�[�g�֑J��
    //    TransitionIdleState();
    //}
}