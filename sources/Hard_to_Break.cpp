#include "Hard_to_Break.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif

Hard_to_Break::Hard_to_Break(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::cution,ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Hard_to_Break::~Hard_to_Break()
{

}

void Hard_to_Break::Update(float elapsedTime)
{
    UpdateTransform();
    Return_orijinal_ObjType(elapsedTime);
    ObjType_effect(elapsedTime);
}

void Hard_to_Break::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Hard_to_Break::Gui()
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
}

Super_hard_to_Break::Super_hard_to_Break(ID3D11Device* device)
{
    Normal = { 0.f,-1.f,0.f };
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_cution,ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
}

Super_hard_to_Break::~Super_hard_to_Break()
{

}

void Super_hard_to_Break::Update(float elapsedTime)
{
   
    UpdateTransform();
    Return_orijinal_ObjType(elapsedTime);
    ObjType_effect(elapsedTime);
}

void Super_hard_to_Break::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Super_hard_to_Break::Gui()
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
}
