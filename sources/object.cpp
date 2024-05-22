#include "object.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include"Graphics/DebugRenderer.h"
using namespace DirectX;

#include "Graphics/graphics.h"

void Object::RayCastGround()
{


    VMCFHT& ince_ray = VMCFHT::instance();
    XMFLOAT3 start = Position;
    //start.y -= 0.05f;
    XMFLOAT3 end = Position;
    end.y -= 0.05f;
    Ray_ObjType type = Ray_ObjType::Stage;
    HitResult hit;

    if (ince_ray.RayCast(start, end, hit, type))
    {
        Position.y = hit.position.y + 0.02f;
        VeloctyY = 0;
    }
    Position.y += VeloctyY;
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




void Object::BaseGui()
{

    if (ImGui::CollapsingHeader("Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("position"))
            {

                XMFLOAT3 pos{ GetPosition() };
                ImGui::InputFloat("Position.x:", &pos.x);
                ImGui::InputFloat("Position.y:", &pos.y);
                ImGui::InputFloat("Position.z:", &pos.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("scale"))
            {
                XMFLOAT3 scale{ GetScale() };
                ImGui::InputFloat("scale.x:", &scale.x);
                ImGui::InputFloat("scale.y:", &scale.y);
                ImGui::InputFloat("scale.z:", &scale.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("angle"))
            {
                XMFLOAT3 angle{GetAngle()};
                ImGui::InputFloat("angle.x:", &angle.x);
                ImGui::InputFloat("angle.y:", &angle.y);
                ImGui::InputFloat("angle.z:", &angle.z);

                ImGui::TreePop();
            }
        }

        if (ImGui::CollapsingHeader("move_obj", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("move_position"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("move_Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                Position.x += pos.x;
                Position.y += pos.y;
                Position.z += pos.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_scale"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("move_scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                Scale.x += scale.x;
                Scale.y += scale.y;
                Scale.z += scale.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_angle"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 angle{};
                ImGui::SliderFloat("move_angle.x:", &angle.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_angle.y:", &angle.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_angle.z:", &angle.z, valu.Min.z, valu.Max.z);
                Angle.x += angle.x;
                Angle.y += angle.y;
                Angle.z += angle.z;
                ImGui::TreePop();
            }
            
        }
    }
    if (ImGui::TreeNode("SpherePos"))
    {
        ImGui::SliderFloat("SpherePos", &spehereLength, 0.f, 5.f);
        ImGui::SliderFloat("SphereRadius", &spehereRadius, 0.f, 5.f);
        /*const char* r = u8"赤は右";
        const char* g = u8"緑は左";
        const char* b = u8"青は前";
        const char* i = u8"黄色は後ろ";*/
        if (ImGui::TreeNode("resultsphere_Type"))
        {
            if (resultsphere_GUI.type == SphereAttribute::Right)
            {
                ImGui::TextUnformatted("red_hit");
            }
            else if (resultsphere_GUI.type == SphereAttribute::Left)
            {
                ImGui::TextUnformatted("green_left");
            }
            else if (resultsphere_GUI.type == SphereAttribute::Front)
            {
                ImGui::TextUnformatted("blue_front");
            }
            else if (resultsphere_GUI.type == SphereAttribute::Backfront)
            {
                ImGui::TextUnformatted("yellow_back");
            }
            ImGui::TreePop();
        }
        ImGui::Checkbox("CustomSphereFlag",&CustomSphereFlag);
        if(CustomSphereFlag)
        if (ImGui::TreeNode("customSphere"))
        {
            DirectX::XMFLOAT3 pos{};
            
            ImGui::SliderFloat3("offsetpos_red",&pos.x,parameter_valu.Min.x*0.5f,parameter_valu.Max.x*0.5f);
            //right
            CustomSpherePos.offsetpos[0].x += pos.x;
            CustomSpherePos.offsetpos[0].y += pos.y;
            CustomSpherePos.offsetpos[0].z += pos.z;
            pos = {};
            ImGui::SliderFloat3("offsetpos_green",&pos.x,parameter_valu.Min.x*0.5f,parameter_valu.Max.x*0.5f);
            //left
            CustomSpherePos.offsetpos[1].x += pos.x;
            CustomSpherePos.offsetpos[1].y += pos.y;
            CustomSpherePos.offsetpos[1].z += pos.z;
            pos = {};
            ImGui::SliderFloat3("offsetpos_blue",&pos.x,parameter_valu.Min.x*0.5f,parameter_valu.Max.x*0.5f);
            //front
            CustomSpherePos.offsetpos[2].x += pos.x;
            CustomSpherePos.offsetpos[2].y += pos.y;
            CustomSpherePos.offsetpos[2].z += pos.z;
            pos = {};
            ImGui::SliderFloat3("offsetpos_yellow",&pos.x,parameter_valu.Min.x*0.5f,parameter_valu.Max.x*0.5f);
            //backfront
            CustomSpherePos.offsetpos[3].x += pos.x;
            CustomSpherePos.offsetpos[3].y += pos.y;
            CustomSpherePos.offsetpos[3].z += pos.z;
            ImGui::TreePop();
        }
        ImGui::Checkbox("IsObject", &isObject);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("RayCastList"))
    {
        ImGui::Checkbox("IsRayCastGimic",&israycast.IsBoundhingBoxVSGimic);
        ImGui::Checkbox("IsRayCastGround",&israycast.IsRayCastGround);
        ImGui::Checkbox("IsRayCastInvisibleWall",&israycast.IsRayCastInvisibleWall);
        ImGui::Checkbox("IsRayCastObject",&israycast.IsRayCastObject);
        ImGui::Checkbox("IsSphereCollition", &israycast.IsSphereCollition);

        ImGui::TreePop();
    }
}

void Object::InvisibleWall_VS_Object()
{
    using namespace DirectX;
    isWall = false;
    VMCFHT& ince_ray = VMCFHT::instance();
    
    Ray_ObjType type = Ray_ObjType::Static_objects;
    XMFLOAT3 start{ this->Position };
    XMFLOAT3 end{this->Position.x+VelocityXZ.x,
                 this->Position.y,
                 this->Position.z+VelocityXZ.y};
    HitResult hit;
    if (ince_ray.RayCast(start, end, hit, type))
    {
        isWall = true;
        XMVECTOR Start{ hit.position.x,hit.position.y,hit.position.z };

        XMVECTOR End{ XMLoadFloat3(&end) };

        XMVECTOR SEvec{ XMVectorSubtract(Start,End) };

        XMVECTOR Normal{ XMLoadFloat3(&hit.normal) };
        XMVECTOR Dot{ XMVector3Dot(Normal, SEvec) };
        float dot = 0;
        XMStoreFloat(&dot, Dot);
        XMVECTOR S{ XMVectorScale(Normal,dot*1.3f) };
        XMFLOAT3 p;
        XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));
       this->Position.x = p.x;
       this->Position.z = p.z;
    }
}

//
//void Object::HitSphere()
//{
//    Objectmanajer& ince = Objectmanajer::incetance();
//    //自分がオブジェクトに接触していたら動けないようにする
//    int count = ince.Get_GameObjCount();
//    isObject = false;
//    for (int i = 0; i < count; i++)
//    {
//        Object* obj = ince.Get_GameObject(i);
//        if (obj == this)continue;
//        XMFLOAT3 pos;
//        if (QuadPlacement_vs_ThisSphere(obj->GetMySphere(), Position))
//        {
//            Position = oldPosition;
//            isObject = true;
//
//            break;
//        }
//        else
//        {
//        }
//
//    }
//}

void Object::CreateSphere()
{
    if (!CustomSphereFlag)
    {
        SphereQuadPlacement spheres(Position, spehereRadius);
        spheres.sphereLength = spehereLength;
        CreateQuadPlacement(spheres);
    }
    else
    {
        SphereQuadPlacement spheres(Position, spehereRadius,CustomSpherePos);
        spheres.sphereLength = spehereLength;
        CreateQuadPlacement(spheres);
    }

}

void Object::CreateQuadPlacement(SphereQuadPlacement& sphere)
{
    
    sphere.pos[0].Spherepos.x += sphere.sphereLength * sphere.sphereLength;//right
    sphere.pos[1].Spherepos.x -= sphere.sphereLength * sphere.sphereLength;//left
    sphere.pos[2].Spherepos.z += sphere.sphereLength * sphere.sphereLength;//front
    sphere.pos[3].Spherepos.z -= sphere.sphereLength * sphere.sphereLength;//back
    DebugRenderer& ince = DebugRenderer::incetance(Graphics::Instance().GetDevice());
    ince.DrawSphere(sphere.pos[0].Spherepos, sphere.SphereRadius, { 1, 0, 0, 1 });
    ince.DrawSphere(sphere.pos[1].Spherepos, sphere.SphereRadius, { 0, 1, 0, 1 });
    ince.DrawSphere(sphere.pos[2].Spherepos, sphere.SphereRadius, { 0, 0, 1, 1 });
    ince.DrawSphere(sphere.pos[3].Spherepos, sphere.SphereRadius, { 1, 1, 0, 1 });
    SetMySphere(sphere);
}
//
//bool Object::QuadPlacement_vs_ThisSphere(const SphereQuadPlacement& sphere, const DirectX::XMFLOAT3& Position)
//{
//    using namespace DirectX;
//
//    XMVECTOR ThisPos{ XMLoadFloat3(&Position) };
//    const int SphereCount = sphere.size;
//    SphereQuadPlacement param{};
//    for (int i = 0; i < SphereCount; i++)
//    {
//        XMVECTOR QuadSpherePos{ XMLoadFloat3(&sphere.pos[i].Spherepos) };
//        XMVECTOR Vec = XMVector3LengthSq(XMVectorSubtract(QuadSpherePos, ThisPos));
//
//        float dist = XMVectorGetX(Vec);
//        float rad = radius + sphere.SphereRadius;
//        if (dist < rad * rad)
//        {
//            return true;
//        }
//    }
//    return false;
//}

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
