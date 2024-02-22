#include "object.h"
#include"StageManager.h"
#include"variable_management_class_for_hit_test.h"
using namespace DirectX;
void Object::ObjType_effect(float elapsedTime, XMFLOAT3& Pos)
{
    StageManager& ince_st = StageManager::incetance();
    VMCFHT& ince_vf = VMCFHT::instance();
    XMFLOAT3 normal{ 0.0f,-1.0f,0.0f };
    float legth=1.0f;
    
    collision_mesh* mesh = ince_st.GetStages(ince_st.GetNowStage())->GetModel()->Get_RaycastCollition();
    switch (old_attribute_state)
    {
    case ObjType::cution:
        //Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
            ince_st.GetStages(ince_st.GetNowStage())->Set_Color({ 1.0f,1.0f,1.0f,1.0f });
        }
        //else Pos.y += Velocty.y;
        color = { 0.0f,1.0f,0.0f,1.0f };
        break;
    case ObjType::Super_cution:
        Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
        }
        else Pos.y += Velocty.y;

        break;
    case ObjType::Hard_to_Break:
        Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
        }
        else Pos.y += Velocty.y;

        break;
    case ObjType::Super_hard_to_Break:
        Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
        }
        else Pos.y += Velocty.y;

        break;
    case ObjType::heavy:
        //Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
            ince_st.GetStages(ince_st.GetNowStage())->Set_Color({ 1.0f,0.0f,0.0f,1.0f });
        }
        //else Pos.y += Velocty.y;
        color = { 1.0f,1.0f,1.0f,1.0f };

        break;
    case ObjType::Super_heavy:
        Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
        }
        else Pos.y += Velocty.y;

        break;
    case ObjType::Fragile:
        Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
        }
        else Pos.y += Velocty.y;

        break;
    case ObjType::Super_fragile:
        Velocty = { 0.f,1.5f * -elapsedTime,0.5f * elapsedTime };
        ince_vf.update(Pos, normal);
        if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
        {
            Velocty = {};
            ince_st.GetStages(ince_st.GetNowStage())->Set_GameObjType(old_attribute_state);
        }
        else Pos.y += Velocty.y;

        break;
    case ObjType::null:

        break;
    default:
        break;
    }

}

void Object::Return_orijinal_ObjType(float elapsedTime)
{

    if (original_attribute_state == old_attribute_state)return;
    else
    {
        ReturnTimer += elapsedTime;
        if (ReturnTimer >= 10.0f)
        {
            old_attribute_state = original_attribute_state;

            ReturnTimer = 0.0f;
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
    //３つの行列を組み合わせて、ワールド座標を作成
    DirectX::XMMATRIX W = C * S * R * T;
    //計算したワールド座標を取り出す
    DirectX::XMStoreFloat4x4(&Transform, W);

}
