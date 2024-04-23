#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
/// <summary>
/// 
/// </summary>
/// <param name="sphere">obj��sphere</param>
/// <param name="Position">player�̈ʒu</param>
/// <param name="radius">player�̔��a</param>
/// <param name="outsphere">���ʕۑ��p</param>
/// <returns></returns>
bool QuadPlacement_vs_PlayerSphere(const Object::SphereQuadPlacement& sphere, const DirectX::XMFLOAT3& Position, float radius, Object::ResultSphereQuadPlacement& outsphere)
{
    using namespace DirectX;

    XMVECTOR ThisPos{ XMLoadFloat3(&Position) };
    const int SphereCount = sphere.size;
    Object::SphereQuadPlacement param{};
    for (int i = 0; i < SphereCount; i++)
    {
        XMVECTOR QuadSpherePos{ XMLoadFloat3(&sphere.pos[i].Spherepos) };
        XMVECTOR Vec = XMVector3LengthSq(XMVectorSubtract(QuadSpherePos, ThisPos));

        float dist = XMVectorGetX(Vec);
        float rad = radius + sphere.SphereRadius;
        if (dist < rad * rad)
        {
            XMStoreFloat3(&outsphere.Spherepos, QuadSpherePos);
            outsphere.type = sphere.pos[i].Type;
            return true;
        }
    }
    return false;
}
//�R���X�g���N�^
Player::Player(ID3D11Device* device)
{
    model = std::make_unique<Model>(device, filename, true);
    const float scale_fcator = 0.001f;	//���f�����傫���̂ŃX�P�[������
    scale = { scale_fcator, scale_fcator, scale_fcator };
}

//�f�X�g���N�^
Player::~Player()
{

}

//�X�V����
void Player::update(float elapsedTime)
{

    if (1)
    {
        //////�ړ����͏���
        inputMove(elapsedTime);

        ////�W�����v���͏���
        inputJump();

        ////���x�����X�V
        updateVelocity(elapsedTime);
    }

    ////���[���h�s��̍X�V
    UpdateTransform();
    //model->UpdateTransform(transform);
}

//�`�揈��
void Player::render(RenderContext* rc)
{
    model->render(rc->deviceContext, transform, 0.0f, { 1.0f,1.0f,1.0f,1.0f });
}


//����ړ�
void Player::inputMove(float elapsedTime)
{
    //�i�s�x�N�g�����擾
    DirectX::XMFLOAT3 moveVec = getMoveVec();

    move(moveVec.x, moveVec.z, this->moveSpeed);

    //���񏈗�
    turn(elapsedTime, moveVec.x, moveVec.z, this->turnSpeed);
}

//���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::getMoveVec() const
{
    //���͏����擾
    gamepad& gamePad = gamepad::Instance();
    float ax = gamePad.thumb_state_lx();//->getAxisLX();
    float ay = gamePad.thumb_state_ly();//->getAxisLY();

    //�J�����������擾
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3 cameraFront = camera.GetFront();//camera->getFront();
    const DirectX::XMFLOAT3 cameraRight = camera.GetRight();

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;

    //�J�����O�����x�N�g����P�ʃx�N�g����
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    }

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraFrontLength;
    }

    //�������͒l���J�����O�����ɔ��f���A�����������J�����E�����ɔ��f���i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = cameraFrontX * ay + cameraRightX * ax;
    vec.z = cameraFrontZ * ay + cameraRightZ * ax;

    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

//�W�����v���͏���
void Player::inputJump()
{
    gamepad& gamePad = gamepad::Instance();
    if (GetKeyState('Z') & 1)
    {
        a++;
        //�W�����v�񐔐���
        if (jumpCount < jumpLimit)
        {
            //�W�����v
            jumpCount++;
            jump(jumpSpeed);
        }
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}
