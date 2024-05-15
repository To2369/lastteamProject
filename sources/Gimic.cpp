#include"Gimic.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"


bool Gimic::Raycast(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, HitResult& Hit)
{

    VMCFHT& ince_ray = VMCFHT::instance();
    return ince_ray.raycast(start, end, this->GetModel(), Hit, this->GetTransform());
}

void Gimic::ResultInfo()
{
    OutputDebugStringA("\n");
    OutputDebugStringA("Position.x: "); OutputDebugStringA(to_string(Position.x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.y: "); OutputDebugStringA(to_string(Position.y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.z: "); OutputDebugStringA(to_string(Position.z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("\n");
    OutputDebugStringA("Scale.x: "); OutputDebugStringA(to_string(Scale.x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.y: "); OutputDebugStringA(to_string(Scale.y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.z: "); OutputDebugStringA(to_string(Scale.z).c_str()); OutputDebugStringA("\n");


    OutputDebugStringA("\n");
}

void Gimic::IsWall_UpdateTransform()
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
    XMMATRIX S = XMMatrixScaling(BoxScale.x, BoxScale.y, BoxScale.z);
    //��]�s��쐬

    XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(BoxAngle.x, BoxAngle.y, BoxAngle.z);
    //�ʒu�s���BoxPosition
    XMMATRIX T = XMMatrixTranslation(BoxPosition.x, BoxPosition.y, BoxPosition.z);
    //4�̍s���g�ݍ��킹�āA���[���h���W���쐬
    DirectX::XMMATRIX W = C * S * R * T;
    //�v�Z�������[���h���W�����o��
    DirectX::XMStoreFloat4x4(&BoxTransform, W);
}

void Gimic::Gimic_effect(Gimic_Type type)
{

#if true
    switch (type)
    {
    case Gimic_Type::Switch:
        if (gimic_VS_Object())
        {
            switchFlag = true;
        }
        else switchFlag = false;
        break;
    case Gimic_Type::Door:
        Gimic_VS_GimicFlagBoot();
        
        if (bootFlag)
        {
            /*ObjType oldtype = this->Get_Old_Objtype(0);
            this->Set_attribute(ObjType::Super_fragile, 0);
            ObjType newtype = this->Get_Old_Objtype(0);
            if (oldtype == newtype)
            {
                this->SetReturnTimer();
            }*/
            FallDown(90,0.1f);

        }
    
        break;
    case Gimic_Type::null:
        break;
    default:
        break;
    }
#endif



}

//�h�A�M�~�b�N���������̏���
void Gimic::FallDown(float MaxAngle,float speed)
{
    falldownAngle+=speed;
    if (falldownAngle >= MaxAngle)
    {
        falldownAngle = MaxAngle;
    }
    Angle.x = XMConvertToRadians(-falldownAngle);
}

