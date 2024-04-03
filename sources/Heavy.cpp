#include"Heavy.h"
#include <objectManajer.h>

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif

using namespace std;
Heavy::Heavy(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::heavy, ObjType::null);

    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Heavy::~Heavy()
{

}

void Heavy::Update(float elapsedTime)
{
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        Gimic* gimic = ince.Get_GameGimic(i);
        if (ince.Bounding_Box_vs_Bounding_Box(this, gimic, true, 0.045f))
        {
            if (Get_isGimic_UpPosNow())
            {
                string g = gimic->GetGimicID();
                Set_GimicType(g);
                break;
            }
        }

    }
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

  /*  if (isGimic_UpPosNow)
    {
        Velocty.y = 0;
    }*/
    if (!Get_isGimic_UpPosNow())RayCastGround();

    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Heavy::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

bool Heavy::gimic_VS_Object()
{

    return false;
}

void Heavy::Gui()
{
    if (ImGui::CollapsingHeader("Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("position"))
            {
             
                XMFLOAT3 pos{GetPosition()};
                ImGui::InputFloat("Position.x:", &pos.x);
                ImGui::InputFloat("Position.y:", &pos.y);
                ImGui::InputFloat("Position.z:", &pos.z);
            
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("scale"))
            {
                XMFLOAT3 scale{GetScale()};
                ImGui::InputFloat("scale.x:", &scale.x);
                ImGui::InputFloat("scale.y:", &scale.y);
                ImGui::InputFloat("scale.z:", &scale.z);
             
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("normal"))
            {
                XMFLOAT3 normal{ Normal};
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
}

Super_Heavy::Super_Heavy(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_heavy, ObjType::null);

    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Super_Heavy::~Super_Heavy()
{
}

void Super_Heavy::Update(float elapsedTime)
{
    
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

   if (!Get_isGimic_UpPosNow())RayCastGround();

    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_Heavy::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Super_Heavy::Gui()
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
    }
}
