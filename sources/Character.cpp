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
