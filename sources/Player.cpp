#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
#include"Graphics/DebugRenderer.h"
#include "Graphics/graphics.h"
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
    Smodel= std::make_unique<Model>(device, Sfilename, true);
    const float scale_fcator = 0.01f;	//���f�����傫���̂ŃX�P�[������
    scale = { scale_fcator, scale_fcator, scale_fcator };
    color = {0,0,0,0};
    isHand = false;
    radius = 0.1f;
    Sposition = { 0,23,0 };
}

//�f�X�g���N�^
Player::~Player()
{

}

//�X�V����
void Player::update(float elapsedTime)
{
    elapsedTime_ = elapsedTime;

    //�ړ����͏���
    inputMove(elapsedTime);

    //�W�����v���͏���
    inputJump();

    //���x�����X�V
    updateVelocity(elapsedTime);

    //�v���C���[�ƃM�~�b�N�̓����蔻��
    CollisionPlayerVsGimics(elapsedTime);

    //���o����
    ExtractionAttribute(elapsedTime);

    Smodel->kefreame = Smodel->getKeyFreame(elapsedTime);
    Smodel->update_animation(*Smodel->kefreame);

    ////���[���h�s��̍X�V
    UpdateTransform();
   
    gamepad& gamePad = gamepad::Instance();
    if (GetKeyState('L'))
    {
        velocity.y = 0;
        position.y = 1;
        velocity.y = 0;
        position.x = 0;
        position.z = 0;
    }

    if (GetKeyState('P'))
    {
        color = {1,1,1,1};
        isHand = true;
    }

    if (GetKeyState('Q'))
    {
        color = { 1,1,1,1 };
        
    }
}

//�`�揈��
void Player::render(RenderContext* rc)
{
    DebugRenderer& debugRenderer = DebugRenderer::incetance(Graphics::Instance().GetDevice());
    Graphics& graphics = Graphics::Instance();
    if(isHand)	graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);
    else	graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(1), nullptr, 0xFFFFFFFF);

    model->render(Graphics::Instance().GetDeviceContext(), transform, 0, color);
    Smodel->render(Graphics::Instance().GetDeviceContext(), Stransform, elapsedTime_, { Scolor });
    //�Փ˔���p�̃f�o�b�O����`��
    debugRenderer.DrawSphere(position, radius, { 1,0,0,1 });

    graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);
}


//����ړ�
void Player::inputMove(float elapsedTime)
{
    //�i�s�x�N�g�����擾
    DirectX::XMFLOAT3 moveVec = getMoveVec();

    move(moveVec.x, moveVec.z, this->moveSpeed);

    //���񏈗�
    turn(elapsedTime, moveVec.x, moveVec.z, this->turnSpeed);

    //�i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

//���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::getMoveVec() const
{
    //���͏����擾
    gamepad& gamePad = gamepad::Instance();
    float ax = gamePad.thumb_state_rx();//->getAxisLX();
    float ay = gamePad.thumb_state_ry();//->getAxisLY();

    //�J�����������擾
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3 cameraFront = camera.GetFront();//camera->getFront();
    const DirectX::XMFLOAT3 cameraRight = camera.GetRight();


    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraRightLength;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;

    //�J�����O�����x�N�g����P�ʃx�N�g����
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength < 0.0f)
    {
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
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
    if (gamePad.button_state(gamepad::button::a))
    {
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

//�v���C���[�ƃM�~�b�N�̓����蔻��
void Player::CollisionPlayerVsGimics(float elapsedTime)
{
    Objectmanajer& objMgr = Objectmanajer::incetance();
    gamepad& ince_pad = gamepad::Instance();
    //�S�ẴM�~�b�N�Ƒ�������ŏՓˏ���

    int objCount = objMgr.Get_GameObjCount();
    for (int i = 0; i < objCount; i++)
    {
        Object* obj = objMgr.Get_GameObject(i);
        //�Փ˔���
        Object::ResultSphereQuadPlacement outsphere;

        if (GetKeyState(' '))//supace�L�[
        {
            if (QuadPlacement_vs_PlayerSphere(obj->GetMySphere(), position, radius, outsphere))
            {
                float objMovesp = velocity.x;
                switch (outsphere.type)
                {
                case Object::SphereAttribute::Right:
                {
                        objMovesp = velocity.x*elapsedTime;
                       // if (objMovesp < 0)
                        if (!obj->GetIsWall() && !obj->GetIsObject())
                            if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                                if (objMovesp < 0)
                                {
                                    obj->SetVelotyXZ({ objMovesp,0 });
                                }
                                else
                                {
                                    obj->SetVelotyXZ({ 0,0 });
                                }
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    color = { 1,1,1,1 };
                    angle.y = DirectX::XMConvertToRadians(270);
                    isHand = true;
                }
                    break;
                case Object::SphereAttribute::Left:
                {
                     objMovesp = velocity.x*elapsedTime;
                   // if (velocity.x * elapsedTime > 0)
                     if (!obj->GetIsWall()&&!obj->GetIsObject())
                        if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                            if (objMovesp>0)
                            {
                                obj->SetVelotyXZ({ objMovesp,0 });
                            }
                            else
                            {
                                obj->SetVelotyXZ({ 0,0 });
                            }
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    angle.y = DirectX::XMConvertToRadians(90);
                    color = { 1,1,1,1 };
                    isHand = true;
                }
                    break;
                case Object::SphereAttribute::Front:

                    objMovesp = velocity.z * elapsedTime;
                    if (!obj->GetIsWall() && !obj->GetIsObject())
                        if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                            if (objMovesp<0)
                            {
                                obj->SetVelotyXZ({ 0,objMovesp });
                            }
                            else
                            {
                                obj->SetVelotyXZ({ 0,0 });
                            }
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    angle.y = DirectX::XMConvertToRadians(180);
                    color = { 1,1,1,1 };
                    break;
                case Object::SphereAttribute::Backfront:

                    objMovesp = velocity.z * elapsedTime;
                  //  if (objMovesp < 0)
                    if (!obj->GetIsWall() && !obj->GetIsObject())
                        if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                            if (objMovesp>0)
                            {
                                obj->SetVelotyXZ({ 0,objMovesp });
                            }
                            else
                            {
                                obj->SetVelotyXZ({ 0,0 });
                            }
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    color = { 1,1,1,1 };
                    angle.y = DirectX::XMConvertToRadians(0);
                    isHand = true;
                    break;
                case Object::SphereAttribute::null:
                    break;
                }
            }
        }
        else
        {
            obj->SetVelotyXZ({ 0,0 });
            color = { 0,0,0,0 };
            isHand = false;
        }

    }
}

//���o����
void Player::ExtractionAttribute(float elapsedTime)
{
    gamepad& gamePad = gamepad::Instance();

    VMCFHT& ince_ray = VMCFHT::instance();
    HitResult hit;
    float lengthpow = -0.1f;
    XMFLOAT3 start{ position.x,position.y + 0.1f,position.z };
    XMFLOAT3 end{ position.x + lengthpow,position.y + 0.1f,position.z + lengthpow };
    DirectX::XMFLOAT3 outpos = { 0,0,0 };
    Ray_ObjType type2 = Ray_ObjType::DaynamicObjects;
    Objectmanajer& objMgr = Objectmanajer::incetance();
    int objCount = objMgr.Get_GameObjCount();
    for (int i = 0; i < objCount; i++)
    {
        //�l�p��
        Object* obj = objMgr.Get_GameObject(i);
        if (ince_ray.RayCast(start, end, hit, type2))
        {
            //���o
            if (gamePad.button_state(gamepad::button::b))
            {
                playerType = obj->Get_Original_Objtype(0);
                break;
            }
            //����
            if (GetKeyState('J'))
            {
                obj->Set_attribute(playerType, 0);

                break;
            }
        }
        //�~����
        else if (objMgr.Sphere_VS_Player(position, radius, obj->GetPosition(), obj->GetRadius(), outpos))
        {
            //���o
            if (gamePad.button_state(gamepad::button::b))
            {
                playerType = obj->Get_Original_Objtype(0);
                break;
            }
            //����
            if (GetKeyState('J'))
            {
                obj->Set_attribute(playerType, 0);
                break;
            }
        }

    }
    if (GetKeyState('K'))
    {
        playerType = Obj_attribute::null;
    }
}