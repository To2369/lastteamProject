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
        if (ince.Bounding_Box_vs_Bounding_Box(this, obj, true, 0.045f))
        {
            break;
        }
    }
}

void Object::RayCastGround()
{
 
    enum
    {
        attribute1 = 0,
        attribute2
    };
    StageManager& ince_st = StageManager::incetance();
    VMCFHT& ince_vf = VMCFHT::instance();
    XMFLOAT3 normal = GetNormal();;
    float legth = 1.0f;
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
    
    enum
    {
        attribute1 = 0,
        attribute2
    };
    {
        switch (old_attribute_state[attribute1])
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

            break;
        case ObjType::Super_fragile:
            this->Destroy();
            break;
        case ObjType::null:

            break;
        default:
            break;
        }
    }

    {
        switch (old_attribute_state[attribute2])
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

            break;
        case ObjType::Super_fragile:
            this->Destroy();
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

    //スケール行列を作成
    XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
    //回転行列作成
    /*XMMATRIX X = XMMatrixRotationX(Angle.x);
    XMMATRIX Y = XMMatrixRotationY(Angle.y);
    XMMATRIX Z = XMMatrixRotationZ(Angle.z);*/
    XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(Angle.x, Angle.y, Angle.z);
    //位置行列を作成
    XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
    //4つの行列を組み合わせて、ワールド座標を作成
    DirectX::XMMATRIX W = C * S * R * T;
    //計算したワールド座標を取り出す
    DirectX::XMStoreFloat4x4(&Transform, W);

}
