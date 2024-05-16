#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
#include"Graphics/DebugRenderer.h"
#include "Graphics/graphics.h"
#include"UIManajer.h"
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
    Smodel = std::make_unique<Model>(device, Sfilename, Sfilename, true);
    const float scale_fcator = 0.01f;	//���f�����傫���̂ŃX�P�[������
    const float Sscale_fcator = 0.005f;
    scale = { scale_fcator, scale_fcator, scale_fcator };
    Sscale = { Sscale_fcator,Sscale_fcator,Sscale_fcator };
    color = {0,0,0,0};
    isHand = false;
    radius = 0.1f;
    Sposition = { 0,1000,0 };
    resetPosition = position;

    Graphics& graphics = Graphics::Instance();
    UIManager& ince_ui = UIManager::incetance();
    //ui�쐬
    {
        
        vector<unique_ptr<UI>>uis;
        ince_ui.CreateUI(graphics.GetDevice(),failepath::UI_Player_Property+L"karu.png",ObjType::cution,"cution", uis);
        ince_ui.CreateUI(graphics.GetDevice(),failepath::UI_Player_Property+L"chookaru.png",ObjType::Super_cution, "Super_cution", uis);
        ince_ui.CreateUI(graphics.GetDevice(),failepath::UI_Player_Property+L"juu.png",ObjType::heavy,"heavy", uis);
        ince_ui.CreateUI(graphics.GetDevice(),failepath::UI_Player_Property+L"choojuu.png",ObjType::Super_heavy,"Super_heavy", uis);
        ince_ui.CreateUI(graphics.GetDevice(),failepath::UI_Player_Property+L"nai.png",ObjType::null,"null", uis);
        ince_ui.UI_move(move(uis));
        ince_ui.CreateCanbas(UI_StringID::CanbasID::Player);
    }
    
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

    pullpushAnime(elapsedTime);
    ////���[���h�s��̍X�V
    UpdateTransform();

    gamepad& gamePad = gamepad::Instance();
    if (position.y<-20)
    {
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = 0;
        direction.x = 0;
        direction.z = 0;
        position.x = resetPosition.x;
        position.y = resetPosition.y+5;
        position.z = resetPosition.z;
    }
    //if (GetAsyncKeyState('U') & 0x8000) // 'K'�L�[�������ꂽ���ǂ������m�F
    //{
    //    if (!wasKeyPressed) // �O��̃t���[����k��������Ă��Ȃ��ꍇ
    //    {

    //        

    //       
    //    }
    //    //wasKeyPressed = true; // wasKeyPressed��true�ɐݒ�
    //}
    //else
    //{
    //    wasKeyPressed = false; // �L�[��������Ă��Ȃ��ꍇ��wasKeyPressed��false�ɐݒ�
    //}
    if (GetKeyState('P'))
    {
        color = { 1,1,1,1 };
        isHand = true;
    }
}

//�`�揈��
void Player::render(RenderContext* rc)
{
    DebugRenderer& debugRenderer = DebugRenderer::incetance(Graphics::Instance().GetDevice());
    Graphics& graphics = Graphics::Instance();
    if(isHand)  model->render(Graphics::Instance().GetDeviceContext(), transform, 0, color);
  
    Smodel->render(Graphics::Instance().GetDeviceContext(), Stransform, elapsedTime_, { Scolor });
    




    //�Փ˔���p�̃f�o�b�O����`��
    debugRenderer.DrawSphere(position, radius, { 1,0,0,1 });

    graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);
}

void Player::updateSyringepos()
{
    XMFLOAT3 cameraeye{ Camera::instance().GetEye() };
    XMFLOAT3 camerafront{ Camera::instance().GetFront() };

    cameraeye.z += camerafront.z * 0.2f;
    cameraeye.x += camerafront.x * 0.2f;
    cameraeye.y = position.y + 0.05f;
    Sposition = cameraeye;
}
//����ړ�
void Player::inputMove(float elapsedTime)
{
    //�i�s�x�N�g�����擾
    DirectX::XMFLOAT3 moveVec = getMoveVec();

    Move(moveVec.x, moveVec.z, this->moveSpeed);

    //�i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    //moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
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
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();//camera->getFront();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();


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
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //�������͒l���J�����O�����ɔ��f���A�����������J�����E�����ɔ��f���i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);

    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

//�W�����v���͏���
void Player::inputJump()
{
    gamepad& gamePad = gamepad::Instance();
    if (gamePad.button_state(gamepad::button::a))//(�X�y�[�X�L�[,A�{�^��)
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

        if (GetKeyState(' '))//(�E�N���b�N�ALB�{�^��)
        {
            if (QuadPlacement_vs_PlayerSphere(obj->GetMySphere(), position, radius, outsphere))
            {
                float objMovesp = velocity.x;
                switch (outsphere.type)
                {
                case Object::SphereAttribute::Right:
                {
                        objMovesp = velocity.x*elapsedTime;
                        if (!obj->GetStatic_Objflag())
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
                   
                    position.x = outsphere.Spherepos.x - 0.06f;
                    position.z = outsphere.Spherepos.z;
                    color = { 1,1,1,1 };
                    angle.y = DirectX::XMConvertToRadians(270);
                    isHand = true;
                }
                    break;
                case Object::SphereAttribute::Left:
                {
                     objMovesp = velocity.x*elapsedTime;
                     if (!obj->GetStatic_Objflag())
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
                   
                     position.x = outsphere.Spherepos.x + 0.06f;
                    position.z = outsphere.Spherepos.z;
                    angle.y = DirectX::XMConvertToRadians(90);
                    color = { 1,1,1,1 };
                    isHand = true;
                }
                    break;
                case Object::SphereAttribute::Front:

                    objMovesp = velocity.z * elapsedTime;
                    if(!obj->GetStatic_Objflag())
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
                    position.z = outsphere.Spherepos.z - 0.06f;
                    angle.y = DirectX::XMConvertToRadians(180);
                    color = { 1,1,1,1 };
                    break;
                case Object::SphereAttribute::Backfront:

                    objMovesp = velocity.z * elapsedTime;
                    if (!obj->GetStatic_Objflag())
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
                    position.z = outsphere.Spherepos.z + 0.06f;
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
            //���o(���N���b�N�ARB�{�^��)
            if (gamePad.button_state(gamepad::button::b))
            {
                updateSyringepos();
                playerType = obj->Get_Original_Objtype(0);
                pullType = true;
                break;
            }
            //����(���N���b�N�ARB�{�^��)
            if (GetKeyState('J'))
            {
                updateSyringepos();
                obj->Set_attribute(playerType, 0);
                pushType = true;
                break;
            }
        }
        //�~����
        else if (objMgr.Sphere_VS_Player(position, radius, obj->GetPosition(), obj->GetRadius(), outpos))
        {
            //���o(���N���b�N�ARB�{�^��)
            if (gamePad.button_state(gamepad::button::b))
            {
                updateSyringepos();
                playerType = obj->Get_Original_Objtype(0);
                pullType = true;
                break;
            }
            //����(���N���b�N�ARB�{�^��)
            if (GetKeyState('J'))
            {
                updateSyringepos();
                obj->Set_attribute(playerType, 0);
                pushType = true;
                break;
            }
        }

    }
    if (GetKeyState('K'))
    {
        playerType = Obj_attribute::null;
    }
}

void Player::pullpushAnime(float elapsedTime)
{
    if (pullType)
    {
        Scolor = { 1,1,1,1 };
        Smodel->kefreame = Smodel->getKeyFreame(elapsedTime, Sanime::pull);
        Smodel->update_animation(*Smodel->kefreame);
        if (Smodel->animation_End)
        {
            pullType = false;
        }
    }
    else if (pushType)
    {
        Scolor = { 1,1,1,1 };
        Smodel->kefreame = Smodel->getKeyFreame(elapsedTime, Sanime::push);
        Smodel->update_animation(*Smodel->kefreame);
        if (Smodel->animation_End)
        {
            pushType = false;
        }
    }
    else
    {
        Scolor = { 0,0,0,0 };
        Smodel->animation_End = false;
        Smodel->stop_animation = false;
        Sposition = { FLT_MAX,FLT_MAX,FLT_MAX};
    }
}