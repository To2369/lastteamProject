#include"Character.h"
#include"StageManager.h"
#include"Mathf.h"
#include <objectManajer.h>
#include"variable_management_class_for_hit_test.h"
//�s��X�V����
void Character::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //�ʒu�s��
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //�R�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;

    // �v�Z�������[���h�s��� transform �Ɏ��o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

//�ړ�����
void Character::move(float vx, float vz, float speed)
{
    // �ړ������x�N�g��������
    direction.x = vx;
    direction.z = vz;

    // �ő呬�x�ݒ�
    maxMoveSpeed = speed;
}

//���񏈗�
void Character::turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�i�ނׂ��i�s�x�N�g�����[���̈����͐��񏈗��̕K�v�Ȃ�
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f)
    {
        return;
    }

    //�i�s�x�N�g���ƒP�ʃx�N�g����
    vx /= length;
    vz /= length;

    //�O�����x�N�g����XZ�������擾���P�ʃx�N�g����
    float frontX = transform._31;   //�O�����x�N�g����X����
    float frontZ = transform._33;   //�O�����x�N�g����Z����
    float frontLength = sqrtf(frontX * frontX + frontZ * frontZ);
    if (frontLength > 0.0f)
    {
        frontX /= frontLength;
        frontZ /= frontLength;
    }

    //���E�`�F�b�N�̂��߂̊O�όv�Z
    float cross = (frontZ * vx) - (frontX * vz);

    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    //2�̃x�N�g���̓��ϒl��-1.0~1.0�ŕ\������܂�.
    float dot = (frontX * vx) + (frontZ * vz);

    //2�̃x�N�g�����d�Ȃ������A��]���x��0.0f�ɂȂ�
    float rot = (1.0f - dot);

    //���܂藣�ꂷ����Ɖ�]���x�������Ȃ肷����̂�speed�ȏ�̉�]���x�ɂ͂Ȃ�Ȃ��悤����
    if (rot > speed)
    {
        rot = speed;
    }

    //�O�ς����̏ꍇ�͉E��]�A���̏ꍇ�͍���]
    if (cross < 0.0f)
    {
        angle.y -= rot;
    }
    else
    {
        angle.y += rot;
    }
}

//�W�����v����
void Character::jump(float speed)
{
    //������̗͐ݒ�
    velocity.y = speed;
}

//���x�����X�V
void Character::updateVelocity(float elapsedTime)
{
    //�������x�X�V����
    updateVerticalVelocity(elapsedTime);
    updateHorizontalVelocity(elapsedTime);

    //�����ړ��X�V����
    updateVerticalMove(elapsedTime);
    updateHorizontalMove(elapsedTime);

}

//�������x�X�V����
void Character::updateVerticalVelocity(float elapsedTime)
{
    //�d�͏���
    velocity.y += gravity * elapsedTime;
}

//�����ړ��X�V����
void Character::updateVerticalMove(float elapsedTime)
{
    //���������̈ړ���
    float moveY = velocity.y * elapsedTime;

    //�L�����N�^�[��Y�������ƂȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0,-1,0 };
    if (moveY < 0.0f)
    {
        //�ړ�����
        position.y += velocity.y * elapsedTime;

        StageManager& ince_st = StageManager::incetance();
        VMCFHT& ince_ray = VMCFHT::instance();
        XMFLOAT3 pos = position;

        ince_ray.update(pos, normal);
        int stagecount = ince_st.GetStageCount();
        for (int i = 0; i < stagecount; i++)
        {
            Object* st = ince_st.GetStages(i);
            collision_mesh& mesh = *st->GetModel()->Get_RaycastCollition();
            XMFLOAT3 start{ position };
            start.y += 0.3f;
            XMFLOAT3 end{ position };
            end.y -= moveY;
            HitResult hit;
            Ray_ObjType type = Ray_ObjType::Stage;
            //�n�ʔ���
            if (ince_ray.RayCast(start, end, hit, type))
            {
                position.y = hit.position.y;
                velocity.y = 0.0f;

                //���n����
                if (!groundedFlag)
                {
                    OnLanding();
                }
                groundedFlag = true;
                break;
            }
            else
            {
                //�󒆂ɕ����Ă���
                position.y += moveY;
                groundedFlag = false;
            }
        }
    }
    else if (moveY > 0.0f)
    {
        //�㏸��
        position.y += moveY;
        groundedFlag = false;
    }
}

//�������x�X�V����
void Character::updateHorizontalVelocity(float elapsedTime)
{
    // ���x�ɗ͂�������Ă�����i0 ����Ȃ�������j�����������s��
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        // ���C�́i�t���[���P�ʂŌv�Z�j
        float friction = this->friction * elapsedTime;

        //�󒆂ɂ���Ƃ��͖��C�͂�����
        if (!groundedFlag)
        {
            friction* airControl;
        }

        if (length > friction)
        {
            // ���C�ɂ�鉡�����̌�������
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        else
        {
            // �������̑��x�����C�͈ȉ��ɂȂ����̂ő��x�𖳌���
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //�ő呬�x�ȉ��Ȃ�����������s��
    if (length <= maxMoveSpeed)
    {
        //�����x�N�g�����[���łȂ��Ȃ�����������s��
        //�[���̏ꍇ�͓��͂���Ă��Ȃ�
        float directionLength = sqrtf(direction.x * direction.x + direction.z * direction.z);
        if (directionLength > 0.0f)
        {
            //�����x(�t���[���P�ʂŌv�Z)
            float acceleration = this->acceleration * elapsedTime;

            //�󒆂ɂ���Ƃ��͉����x������
            if (!groundedFlag)
            {
                acceleration *= airControl;
            }

            //��������
            //�����x�N�g���ɉ����x���X�P�[�����O�����l�𑬓x�x�N�g���ɉ��Z
            velocity.x += direction.x * acceleration;
            velocity.z += direction.z * acceleration;

            //�ő呬�x����
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                // �����x�N�g���ɍő�ړ����x���X�P�[�����O�����l�𑬓x�x�N�g���ɑ��
                float vx = velocity.x / length;
                float vz = velocity.z / length;
                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
        }
    }
}

//�����ړ��X�V����
void Character::updateHorizontalMove(float elapsedTime)
{
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //�v�Z�p�̈ړ���̑��x
        float moveX = velocity.x * elapsedTime;
        float moveZ = velocity.z * elapsedTime;

        HitResult hit;
        XMFLOAT3 start{ position.x,position.y+1.0f,position.z };
        XMFLOAT3 end{ position.x + moveX,position.y+1.0f,position.z + moveZ };
        Ray_ObjType type = Ray_ObjType::Static_objects;
        VMCFHT& ins_ray = VMCFHT::instance();
        Objectmanajer& objMgr = Objectmanajer::incetance();
        int count = objMgr.Get_GameObjCount();
        for (int i = 0; i < count; i++)
        {
            Object* obj = objMgr.Get_GameObject(i);
            Object::SphereQuadPlacement spherePos;
            if (ins_ray.RayCast(start, end, hit, type))
            {

                //�ǂ܂ł̃x�N�g��
                DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
                DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
                DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

                //�ǂ̖@��
                DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

                //���˃x�N�g����@���Ɏˉe
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

                //�␳�ʒu�̌v�Z
                DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
                DirectX::XMFLOAT3 collectPosition;//p
                DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
                HitResult hit2;
                if (!ins_ray.RayCast(start, end, hit2, type))
                {
                    //�ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
                    position.x = collectPosition.x;
                    position.z = collectPosition.z;
                    break;
                }
                else
                {
                    position.x = hit2.position.x;
                    position.z = hit2.position.z;
                    break;
                }
            }
            else
            {
                // �ړ�����
                position.x += moveX;
                position.z += moveZ;
                break;
            }
        }
    }
}
void Character::RayVsCharacter(float moveX, float moveZ)
{

    //DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
    //DirectX::XMFLOAT3 end = { position.x + moveX,position.y,position.z + moveZ };

    //// Objectmanajer& objMgr = Objectmanajer::incetance();

    //StageManager& stMgr = StageManager::incetance();
    //VMCFHT& ins_ray = VMCFHT::instance();
    ////int count = objMgr.Get_GameStatic_ObjectCount();
    //int stagecount = stMgr.GetStageCount();
    //bool move = true;
    //for (int i = 0; i < stagecount; i++)
    //{
    //    //���C�L���X�g�ɂ��ǔ���
    //    Object* st = stMgr.GetStages(i);
    //    collision_mesh& mesh = *st->GetModel()->Get_RaycastCollition();
    //    //XMFLOAT3 start{ position };
    //    //start.y += 0.3f;
    //    //XMFLOAT3 end{ position };
    //    //end.y -= moveY;
    //    HitResult hit;
    //    Ray_ObjType type = Ray_ObjType::Stage;
    //    if (ins_ray.RayCast(start, end, hit, type))
    //    {
    //        //�ǂ܂ł̃x�N�g��
    //        DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    //        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
    //        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

    //        //�ǂ̖@��
    //        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

    //        //���˃x�N�g����@���Ɏˉe
    //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

    //        //�␳�ʒu�̌v�Z
    //        DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
    //        DirectX::XMFLOAT3 collectPosition;//p
    //        DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

    //        //�ǂ�������փ��C�L���X�g
    //        HitResult hit2;
    //        if (!ins_ray.RayCast(start, end, hit2, type))
    //        {
    //            //�ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
    //            position.x = collectPosition.x;
    //            position.z = collectPosition.z;
    //            break;
    //        }
    //        else
    //        {
    //            position.x = hit2.position.x;
    //            position.z = hit2.position.z;
    //        }
    //        break;
    //    }
    //}
}
////�s��X�V����
//void Character::UpdateTransform()
//{
//    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
//      { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
//      { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
//      { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
//      { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
//    };
//    const float scale_factor = 0.01f;
//    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
//    //�X�P�[���s����쐬
//    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//
//    //��]�s����쐬
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//
//    //�ʒu�s����쐬
//    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//
//    //4�̍s���g�ݍ��킹�B���[���h�s����쐬
//    DirectX::XMMATRIX W = C * S * R * T;
//
//    //�v�Z�������[���h�s������o��
//    DirectX::XMStoreFloat4x4(&transform, W);
//}
//
////�ړ�����
//void Character::Move(float vx, float vz, float speed)
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
//void Character::Turn(float elapsedTime, float vx, float vz, float speed)
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
//    float frontX = sinf(angle.y);//PLAYER
//    float frontZ = cosf(angle.y);//PLAYER
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
//        angle.y += -rot;
//    }
//    else
//    {
//        angle.y += rot;
//    }
//}
//
////�W�����v����
//void Character::Jump(float speed)
//{
//    //������̗͂�ݒ�  
//    velocity.y = speed;
//}
//
////���͏����X�V
//void Character::UpdateVelocity(float elapsedTime)
//{
//    //�o�߃t���[��
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    //�������͍X�V����
//    UpdateVerticalVelocity(elapsedTime);
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
//void Character::UpdateVerticalVelocity(float elapsedTime)
//{
//    //�d�͏���
//    velocity.y += gravity * elapsedTime * 60.0f;
//}
//
////�������͍X�V����
//void Character::UpdateHorizontalVelocity(float elapsedFrame)
//{
//    //XZ���ʂ̑��͂���������
//    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);//DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(velVel));
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
//            float vx = velocity.x / length;
//            float vz = velocity.z / length;
//
//            velocity.x -= vx * friction;
//            velocity.z -= vz * friction;
//        }
//        else//�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͖�����
//        {
//            //���x�x�N�g�����O�ɂ���
//            velocity.x = 0.0f;
//            velocity.z = 0.0f;
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
//            velocity.x += moveVecX * acceleration;
//            velocity.z += moveVecZ * acceleration;
//
//            //�ő呬�x����
//            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
//            if (length > maxMoveSpeed)
//            {
//                float vx = velocity.x / length;
//                float vz = velocity.z / length;
//                velocity.x = vx * maxMoveSpeed;
//                velocity.z = vz * maxMoveSpeed;
//            }
//
//            //�����ł����������Ȃ��悤�ɂ���
//            if (isGround && slopeRate > 0.0f)
//            {
//                //�Ζʂł̗������x-���ړ��̒���*�o�߃t���[��
//                velocity.y -= length * slopeRate * elapsedFrame;
//            }
//        }
//    }
//    //�ړ��x�N�g�������Z�b�g
//    moveVecX = 0.0f;
//    moveVecZ = 0.0f;
//}
//
////�����ړ��X�V����
//void Character::UpdateVerticalMove(float elapsedTime)
//{
//    //���������̈ړ���
//    float my = velocity.y * elapsedTime;
//
//    slopeRate = 0.0f;
//
//    //�L�����N�^�[��Y�������ƂȂ�@���x�N�g��
//    DirectX::XMFLOAT3 normal = { 0,1,0 };
//
//    //������
//    
//    if (my < 0.0f)
//    {
//        //���C�̊J�n�ʒu�͑�����菭����
//        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
//        //���C�̏I�_�ʒu�͈ړ���̈ʒu
//        DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };
//
//        DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&start);
//        DirectX::XMVECTOR EndVec = DirectX::XMLoadFloat3(&end);
//        DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(EndVec, startVec);
//
//        DirectX::XMFLOAT3 p;
//        DirectX::XMStoreFloat3(&p, vec);
//
//        //���C�L���X�g�ɂ��n�ʔ���
//        StageManager& ince_st = StageManager::incetance();
//        VMCFHT& ince_ray = VMCFHT::instance();
//        XMFLOAT3 pos = position;
//        ince_ray.update(pos, normal);
//        Object* st = ince_st.GetStages(ince_st.GetNowStage());
//        collision_mesh& mesh = *st->GetModel()->Get_RaycastCollition();
//        Intersection inter{};
//        bool hit = ince_ray.raycast(mesh, st->GetTransform(), inter, 100.0f);
//        if (hit)
//        {
//            //�@���x�N�g���擾
//            normal = inter.intersection_normal;
//
//            //�n�ʂɐڑ����Ă���
//            position.y = inter.intersection_position.y;
//            
//                
//            //��]
//            //angle.y -= int
//
//            //���n����
//            if (!isGround)
//            {
//                OnLanding();
//            }
//            isGround = true;
//            velocity.y = 0.0f;
//
//            //�X�Η��̌v�Z
//           /* float normalLengthXZ = sqrtf(inter.intersection_normal.x * inter.intersection_normal.x + inter.intersection_normal.z * inter.intersection_normal.z);
//            slopeRate = 1.0f - (inter.intersection_normal.y / (normalLengthXZ + inter.intersection_normal.y));*/
//        }
//        else
//        {
//            //�󒆂ɕ����Ă���
//            position.y += my;
//            isGround = false;
//        }
//    }
//    //�㏸��
//    else if (my > 0.0f)
//    {
//        position.y += my;
//        isGround = false;
//    }
//    
//    //�n�ʂ̌����ɓY���悤��XZ����]
//    {
//        ////Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
//        //float x = atan2f(normal.z, normal.y);//normal.z / normal.y;
//        //float z = -atan2f(normal.x, normal.y);//normal.x / normal.y;
//
//        ////���`�⊮�Ŋ��炩�ɂ���
//        //angle.x = Mathf::Leap(angle.x, x, 0.1f);
//        //angle.z = Mathf::Leap(angle.z, z, 0.1f);
//    }
//}
//
////�����ړ��X�V
//void Character::UpdateHorizontalMove(float elapsedTime)
//{
//    ////�������͗ʌv�Z
//    //float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
//    //if (velocityLengthXZ > 0.0f)
//    //{
//    //    //�����ړ��l
//    //    float mx = velocity.x * elapsedTime;
//    //    float mz = velocity.z * elapsedTime;
//
//    //    //���C�̊J�n�ʒu�ƏI�_�ʒu
//    //    DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
//    //    DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };
//
//    //    //���C�L���X�g�ɂ��ǔ���
//    //    HitResult hit;
//    //    if (StageManager::incetance().RayCast(start, end, hit))
//    //    {
//    //        //�ǂ܂ł̃x�N�g��
//    //        DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
//    //        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//    //        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//    //        //�ǂ̖@��
//    //        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//    //        //���˃x�N�g����@���Ɏˉe
//    //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//
//    //        //�␳�ʒu�̌v�Z
//    //        DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//    //        DirectX::XMFLOAT3 collectPosition;//p
//    //        DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//    //        //�ǂ�������փ��C�L���X�g
//    //        HitResult hit2;
//    //        if (!StageManager::incetance().RayCast(hit.position, collectPosition, hit2))
//    //        {
//    //            //�ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
//    //            position.x = collectPosition.x;
//    //            position.z = collectPosition.z;
//    //        }
//    //        else
//    //        {
//    //            position.x = hit2.position.x;
//    //            position.z = hit2.position.z;
//    //        }
//    //    }
//    //    else
//    //    {
//    //        //�ړ�
//    //        position.x += mx;
//    //        position.z += mz;
//    //    }
//    //}
//}