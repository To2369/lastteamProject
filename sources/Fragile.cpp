#include "Fragile.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
Fragile::Fragile(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Fragile,ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Fragile::~Fragile()
{
}

void Fragile::Update(float elapsedTime)
{
 
    color = InitColor();
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (!Get_isGimic_UpPosNow())RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}


void Fragile::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Fragile::Gui()
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

Super_fragile::Super_fragile(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_fragile, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Super_fragile::~Super_fragile()
{

}

void Super_fragile::Update(float elapsedTime)
{
   
    color = InitColor();
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (!Get_isGimic_UpPosNow())RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_fragile::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Super_fragile::Gui()
{
    if (ImGui::CollapsingHeader("Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("_position"))
            {

                XMFLOAT3 pos{ GetPosition() };
                ImGui::InputFloat("_Position.x:", &pos.x);
                ImGui::InputFloat("_Position.y:", &pos.y);
                ImGui::InputFloat("_Position.z:", &pos.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("_scale"))
            {
                XMFLOAT3 scale{ GetScale() };
                ImGui::InputFloat("_scale.x:", &scale.x);
                ImGui::InputFloat("_scale.y:", &scale.y);
                ImGui::InputFloat("_scale.z:", &scale.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("_normal"))
            {
                XMFLOAT3 normal{ Normal };
                ImGui::InputFloat("_normal.x:", &normal.x);
                ImGui::InputFloat("_normal.y:", &normal.y);
                ImGui::InputFloat("_normal.z:", &normal.z);

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
