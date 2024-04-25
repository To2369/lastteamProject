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


void Object::RayCastGround()
{


    StageManager& ince_st = StageManager::incetance();
    VMCFHT& ince_vf = VMCFHT::instance();
    XMFLOAT3 normal = { 0.f,-1.f,0.f };
    float legth = 5.0f;
    XMFLOAT3 pos = Position;
    float start_point = Scale.y * 0.005f;
    if (start_point < 0.1f) start_point = 0.1f;
    else if (start_point > 0.5f)start_point = 0.5f;
    pos.y += -start_point;
    ince_vf.update(pos, normal);
    int stagecount = ince_st.GetStageCount();
    for (int i = 0; i < stagecount; i++)
    {
        collision_mesh* mesh = ince_st.GetStages(i)->GetModel()->Get_RaycastCollition();
        if (ince_vf.raycast(*mesh, ince_st.GetStages(i)->GetTransform(), result_intersection, legth))
        {
            VeloctyY = 0.0f;
            break;
        }
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
            if (ImGui::TreeNode("normal"))
            {
                XMFLOAT3 normal{ Normal };
                ImGui::InputFloat("normal.x:", &normal.x);
                ImGui::InputFloat("normal.y:", &normal.y);
                ImGui::InputFloat("normal.z:", &normal.z);

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
            if (ImGui::TreeNode("move_normal"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 normal{ Normal };
                ImGui::SliderFloat("move_normal.x:", &normal.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_normal.y:", &normal.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_normal.z:", &normal.z, valu.Min.z, valu.Max.z);

                ImGui::TreePop();
            }
        }
    }
    if (ImGui::TreeNode("SpherePos"))
    {
        ImGui::SliderFloat("SpherePos", &Mysphere.sphereLength, 0.f, 1.f);
        /*const char* r = u8"赤は右";
        const char* g = u8"緑は左";
        const char* b = u8"青は前";
        const char* i = u8"黄色は後ろ";*/
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
}


void Object::HitSphere()
{
    Objectmanajer& ince = Objectmanajer::incetance();
    //自分がオブジェクトに接触していたら動けないようにする
    int count = ince.Get_GameObjCount();
    for (int i = 0; i < count; i++)
    {
        Object* obj = ince.Get_GameObject(i);
        if (obj == this)continue;
        ResultSphereQuadPlacement resultSphere{};
        if (QuadPlacement_vs_ThisSphere(obj->GetMySphere(), Position, resultSphere))
        {
            resultsphere_GUI = resultSphere;
            SetResultSphere(resultSphere);
            break;
        }
        else
        {
            resultsphere_GUI = resultSphere;
            SetResultSphere(resultSphere);
        }

    }
}

void Object::CreateQuadPlacement(SphereQuadPlacement& sphere)
{
    RenderContext& rc = RenderContext::incetance();

    sphere.pos[0].Spherepos.x += sphere.sphereLength * sphere.sphereLength;//right
    sphere.pos[1].Spherepos.x -= sphere.sphereLength * sphere.sphereLength;//left
    sphere.pos[2].Spherepos.z += sphere.sphereLength * sphere.sphereLength;//front
    sphere.pos[3].Spherepos.z -= sphere.sphereLength * sphere.sphereLength;//back
    DebugRenderer& ince = DebugRenderer::incetance(rc.device);
    ince.DrawSphere(sphere.pos[0].Spherepos, sphere.SphereRadius, { 1, 0, 0, 1 });
    ince.DrawSphere(sphere.pos[1].Spherepos, sphere.SphereRadius, { 0, 1, 0, 1 });
    ince.DrawSphere(sphere.pos[2].Spherepos, sphere.SphereRadius, { 0, 0, 1, 1 });
    ince.DrawSphere(sphere.pos[3].Spherepos, sphere.SphereRadius, { 1, 1, 0, 1 });
    SetMySphere(sphere);
}

bool Object::QuadPlacement_vs_ThisSphere(const SphereQuadPlacement& sphere, const DirectX::XMFLOAT3& Position, ResultSphereQuadPlacement& outsphere)
{
    using namespace DirectX;

    XMVECTOR ThisPos{ XMLoadFloat3(&Position) };
    const int SphereCount = sphere.size;
    SphereQuadPlacement param{};
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
