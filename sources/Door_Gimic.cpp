#include"Gimic.h"
#include"objectManajer.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
Door::Door(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::Super_hard_to_Break, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    Scale = { 0.05f,0.5f,0.05f };
    Gimic_type = Gimic_Type::Door;
    MyGimicFlagType = Gimic_Type::Switch;
    Normal = { 0.f,1.f,0.f };
    UpdateTransform();
}

void Door::Update(float elapsedTime)
{
    color = InitColor();
    ObjType_effect(elapsedTime);
    Gimic_effect(Gimic_Type::Door);
    Return_orijinal_ObjType(elapsedTime);
    UpdateTransform();
}

void Door::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
  
}

void Door::Gimic_VS_GimicFlagBoot()
{
#if 1
    bootFlag.clear();
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        Gimic* gimic = ince.Get_GameGimic(i);
        if (this->Get_GimicType() == gimic->Get_GimicType())continue;
        if (this->GetGimicID() != gimic->GetGimicID())continue;
        if (gimic->Get_GimicType() == this->GetMyGimicFlagType() && gimic->Get_SwitchFlag())
        {
            this->SetBootFlag(true);
        }

    }
#endif
  
}

void Door::Gui()
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
            if (ImGui::TreeNode("position"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                Position.x += pos.x;
                Position.y += pos.y;
                Position.z += pos.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("scale"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                Scale.x += scale.x;
                Scale.y += scale.y;
                Scale.z += scale.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("normal"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 normal{ Normal };
                ImGui::SliderFloat("normal.x:", &normal.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("normal.y:", &normal.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("normal.z:", &normal.z, valu.Min.z, valu.Max.z);

                ImGui::TreePop();
            }
        }
        if (ImGui::TreeNode("Door_BootChack"))
        {
            int count = bootFlag.size();
            for (int i = 0; i < count; i++)
            {
                bool f = GetBootFlag(i);
                ImGui::Checkbox("Door_Boot",&f);
            }
            ImGui::TreePop();
        }
    }
    const int buffer = 256;
    ImGui::InputText("ID", const_cast<char*>(ID.c_str()), buffer);
    ImGui::Text(ID.c_str());
}
