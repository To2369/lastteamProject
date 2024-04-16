#include "object.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
using namespace DirectX;

void Object::box_Collition_obj()
{

    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        Object* obj = ince.Get_GameGimic(i);

        if (obj == this)continue;
        if (ince.Bounding_Box_vs_Bounding_Box(this, obj, false, 0.045f))
        {
            break;
        }
    }
}

void Object::RayCastGround()
{
 
    
    StageManager& ince_st = StageManager::incetance();
    VMCFHT& ince_vf = VMCFHT::instance();
    XMFLOAT3 normal = GetNormal();;
    float legth = 5.0f;
    XMFLOAT3 pos = Position;
    float start_point = Scale.y*0.005f;
    if (start_point < 0.1f) start_point = 0.1f;
    else if (start_point > 0.5f)start_point = 0.5f;
    pos.y += -start_point;
    ince_vf.update(pos, normal);
    collision_mesh* mesh = ince_st.GetStages(ince_st.GetNowStage())->GetModel()->Get_RaycastCollition();
    if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
    {
        Velocty.y =0.0f;
    }
    else
    {
        Position.y += Velocty.y;
    }
}

void Object::ObjType_effect(float elapsedTime)
{
    
    enum class Num
    {
        attribute1 = 0,
        attribute2
    };
    {
        switch (old_attribute_state[static_cast<int>(Num::attribute1)])
        {
        case ObjType::cution:

          
            break;
        case ObjType::Super_cution:

            break;
        case ObjType::Hard_to_Break:

            break;
        case ObjType::Super_hard_to_Break:


            break;
        case ObjType::heavy:


            break;
        case ObjType::Super_heavy:

            break;
        case ObjType::Fragile:
            if (this->Get_Original_Objtype(0) == ObjType::Crack)this->Destroy();
            break;
        case ObjType::Super_fragile:
            this->SetColor({0.f,0.f,0.f,0.f});
            break;
        case ObjType::Crack:
            
            break;
        case ObjType::null:

            break;
        default:
            break;
        }
    }

    {
        switch (old_attribute_state[static_cast<int>(Num::attribute2)])
        {
        case ObjType::cution:
            break;
        case ObjType::Super_cution:
            break;
        case ObjType::heavy:
            break;
        case ObjType::Super_heavy:
            break;
        case ObjType::Hard_to_Break:

            break;
        case ObjType::Super_hard_to_Break:

            break;
     
        case ObjType::Fragile:

            break;
        case ObjType::Super_fragile:
            this->SetColor({ 0.f,0.f,0.f,0.f });
            break;
        case ObjType::Crack:

            break;
        case ObjType::null:

            break;
        default:
            break;
        }
    }
}

void Object::Return_orijinal_ObjType(float elapsedTime)
{
    const int count = 2;

    for (int i = 0; i < count; i++)
    {
        if (original_attribute_state[i] == old_attribute_state[i])return;
        else
        {
            ReturnTimer[i] += elapsedTime;
            if (ReturnTimer[i] >= 10.0f)
            {
                old_attribute_state[i] = original_attribute_state[i];
                ReturnTimer[i] = 0.0f;
            }
        }
    }

}


void Object::UpdateTransform()
{
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
      { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
      { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
      { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
      { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    const float scale_factor = 0.01f;
    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };

    //�X�P�[���s����쐬
    XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
    //��]�s��쐬
    /*XMMATRIX X = XMMatrixRotationX(Angle.x);
    XMMATRIX Y = XMMatrixRotationY(Angle.y);
    XMMATRIX Z = XMMatrixRotationZ(Angle.z);*/
    XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(Angle.x, Angle.y, Angle.z);
    //�ʒu�s����쐬
    XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
    //4�̍s���g�ݍ��킹�āA���[���h���W���쐬
    DirectX::XMMATRIX W = C * S * R * T;
    //�v�Z�������[���h���W�����o��
    DirectX::XMStoreFloat4x4(&Transform, W);

}

////�ړ�����
//void Object::Move(float vx, float vz, float speed)
//{
//    //�ړ������x�N�g����ݒ�
//    moveVecX = vx;
//    moveVecZ = vz;
//
//    //�ő呬�x�ݒ�
//    maxMoveSpeed = speed;
//}
//
////���񏈗�
//void Object::Turn(float elapsedTime, float vx, float vz, float speed)
//{
//    speed *= elapsedTime;
//    float length = sqrtf(vx * vx + vz * vz);
//    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
//    if (length < 0.001f)
//    {
//        return;
//    }
//
//    //�i�s�x�N�g����P�ʉ�
//    vx /= length;
//    vz /= length;
//
//    //���g�̉�]�l����O���������߂�
//    float frontX = sinf(Angle.y);//PLAYER
//    float frontZ = cosf(Angle.y);//PLAYER
//    //��]�p�����߂邽�߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
//    float dot = (frontX * vx) + (frontZ * vz);
//    float rot = 1.0f - dot;
//
//    if (rot > speed)
//    {
//        rot = speed;
//    }
//    //���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
//    float croos = (frontZ * vx) - (frontX * vz);
//
//    //�QD�̊O�ϔ��肪���̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
//    //���E������s�����ɂ���č��E�����I������
//    if (croos < 0.0f)
//    {
//        Angle.y += -rot;
//    }
//    else
//    {
//        Angle.y += rot;
//    }
//}
//
////�W�����v����
//void Object::Jump(float speed)
//{
//    //������̗͂�ݒ�  
//    Velocty.y = speed;
//}
//
////���͏����X�V
//void Object::UpdateVelocity(float elapsedTime)
//{
//    //�o�߃t���[��
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    //�������͍X�V����
//    UpdateVerticalVelocity(elapsedFrame);
//
//    //�������͍X�V����
//    UpdateHorizontalVelocity(elapsedFrame);
//
//    //�����ړ��X�V����
//    UpdateVerticalMove(elapsedTime);
//
//    //�����ړ��X�V����
//    UpdateHorizontalMove(elapsedTime);
//}
//
////�������͍X�V����
//void Object::UpdateVerticalVelocity(float elapsedFrame)
//{
//    //�d�͏���
//    Velocty.y += gravity * elapsedFrame;
//}
//
////�������͍X�V����
//void Object::UpdateHorizontalVelocity(float elapsedFrame)
//{
//    //XZ���ʂ̑��͂���������
//    float length = sqrtf(Velocty.x * Velocty.x + Velocty.z * Velocty.z);//DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(velVel));
//    if (length > 0.0f)
//    {
//        //���C��
//        float friction = this->friction * elapsedFrame;
//
//        //�󒆂ɂ���Ƃ��͖��C�͂����炷
//        if (!isGround)friction *= airControl;
//
//        //���C�ɂ�鉡�����̌�������
//        if (length > friction)
//        {
//            float vx = Velocty.x / length;
//            float vz = Velocty.z / length;
//
//            Velocty.x -= vx * friction;
//            Velocty.z -= vz * friction;
//        }
//        else
//            //�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͖�����
//        {
//            //���x�x�N�g�����O�ɂ���
//            Velocty.x = 0.0f;
//            Velocty.z = 0.0f;
//        }
//    }
//
//    //XZ���ʂ̑��͂���������
//    if (length <= maxMoveSpeed)
//    {
//        //�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
//        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
//        if (moveVecLength > 0.0f)
//        {
//            //������
//            float acceleration = this->acceleration * elapsedFrame;
//
//            //�󒆂ɂ���Ƃ��͉����͂����炷
//            if (!isGround)acceleration *= airControl;
//
//            //�ړ��x�N�g���ɂ���������
//            Velocty.x += moveVecX * acceleration;
//            Velocty.z += moveVecZ * acceleration;
//
//            //�ő呬�x����
//            float length = sqrtf(Velocty.x * Velocty.x + Velocty.z * Velocty.z);
//            if (length > maxMoveSpeed)
//            {
//                float vx = Velocty.x / length;
//                float vz = Velocty.z / length;
//                Velocty.x = vx * maxMoveSpeed;
//                Velocty.z = vz * maxMoveSpeed;
//            }
//
//            //�����ł����������Ȃ��悤�ɂ���
//            if (isGround && slopeRate > 0.0f)
//            {
//                //�Ζʂł̗������x-���ړ��̒���*�o�߃t���[��
//                Velocty.y -= length * slopeRate * elapsedFrame;
//            }
//        }
//    }
//    //�ړ��x�N�g�������Z�b�g
//    moveVecX = 0.0f;
//    moveVecZ = 0.0f;
//}
//
////�����ړ��X�V����
//void Object::UpdateVerticalMove(float elapsedTime)
//{
//    //���������̈ړ���
//    float my = Velocty.y * elapsedTime;
//
//    slopeRate = 0.0f;
//
//    //�L�����N�^�[��Y�������ƂȂ�@���x�N�g��
//    DirectX::XMFLOAT3 normal = { 0,1,0 };
//
//    //������
//    if (my < 0.0f)
//    {
//        //���C�̊J�n�ʒu�͑�����菭����
//        DirectX::XMFLOAT3 start = { Position.x,Position.y + stepOffset,Position.z };
//        //���C�̏I�_�ʒu�͈ړ���̈ʒu
//        DirectX::XMFLOAT3 end = { Position.x,Position.y + my,Position.z };
//
//        //���C�L���X�g�ɂ��n�ʔ���
//        HitResult hit;
//        //�X�e�[�W�����܂ŃR�����g
//        //if (StageManager::Instance().RayCast(start, end, hit))
//        //{
//        //    //�@���x�N�g���擾
//        //    normal = hit.normal;
//
//        //    //�n�ʂɐڑ����Ă���
//        //    position = hit.position;
//
//
//        //    //��]
//        //    angle.y -= hit.rotation.y;
//
//        //    //���n����
//        //    if (!isGround)
//        //    {
//        //        OnLanding();
//        //    }
//        //    isGround = true;
//        //    velocity.y = 0.0f;
//
//        //    //�X�Η��̌v�Z
//        //    float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
//        //    slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
//        //}
//        //else
//        //{
//        //    //�󒆂ɕ����Ă���
//        //    position.y += my;
//        //    isGround = false;
//        //}
//    }
//    //�㏸��
//    else if (my > 0.0f)
//    {
//        Position.y += my;
//        isGround = false;
//    }
//
//    //�n�ʂ̌����ɓY���悤��XZ����]
//    {
//        //Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
//        //angle.x =  atan2f(normal.z, normal.y);//normal.z / normal.y;
//        //angle.z = -atan2f(normal.x,normal.y);//normal.x / normal.y;
//
//        float x = atan2f(normal.z, normal.y);//normal.z / normal.y;
//        float z = -atan2f(normal.x, normal.y);//normal.x / normal.y;
//
//        //���`�⊮�Ŋ��炩�ɂ���
//        Angle.x = Mathf::Leap(Angle.x, x, 0.1f);
//        Angle.z = Mathf::Leap(Angle.z, z, 0.1f);
//    }
//}
//
////�����ړ��X�V����
//void Object::UpdateHorizontalMove(float elapsedTime)
//{
//    //�������͗ʌv�Z
//    float velocityLengthXZ = sqrtf(Velocty.x * Velocty.x + Velocty.z * Velocty.z);
//    if (velocityLengthXZ > 0.0f)
//    {
//        //�����ړ��l
//        float mx = Velocty.x * elapsedTime;
//        float mz = Velocty.z * elapsedTime;
//
//        //���C�̊J�n�ʒu�ƏI�_�ʒu
//        DirectX::XMFLOAT3 start = { Position.x,Position.y + stepOffset,Position.z };
//        DirectX::XMFLOAT3 end = { Position.x + mx,Position.y + stepOffset,Position.z + mz };
//
//        //���C�L���X�g�ɂ��ǔ���
//        HitResult hit;
//        //if (StageManager::instance().RayCast(start, end, hit))
//        //{
//        //    //�ǂ܂ł̃x�N�g��
//        //    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
//        //    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//        //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//        //    //�ǂ̖@��
//        //    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//        //    //���˃x�N�g����@���Ɏˉe
//        //    DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//
//        //    //�␳�ʒu�̌v�Z
//        //    DirectX::XMVECTOR CollectPosition/*S*/ = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//        //    DirectX::XMFLOAT3 collectPosition;//p
//        //    DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//        //    //���˃x�N�g��
//        //    //DirectX::XMVECTOR CollectPosition/*S*/ = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//        //    //DirectX::XMFLOAT3 collectPosition;//p
//        //    //DirectX::XMStoreFloat3(&collectPosition, DirectX::XMVectorAdd(End, CollectPosition));
//        //    //DirectX::XMVECTOR reflect = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&collectPosition), DirectX::XMLoadFloat3(&hit.position));
//
//        //    //�ǂ�������փ��C�L���X�g
//        //    HitResult hit2;
//        //    if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
//        //    {
//        //        //�ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
//        //        position.x = collectPosition.x;
//        //        position.z = collectPosition.z;
//        //    }
//        //    else
//        //    {
//        //        position.x = hit2.position.x;
//        //        position.z = hit2.position.z;
//        //    }
//        //}
//        //else
//        //{
//        //    //�ړ�
//        //    position.x += mx;
//        //    position.z += mz;
//        //}
//    }
//}