#include"Crack.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
Crack::Crack(ID3D11Device*device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_heavy,ObjType::Crack);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };
    UpdateTransform();
}

void Crack::Update(float elapsedTime)
{
    color = InitColor();
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();
    if (!Get_isGimic_UpPosNow())RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();

}

void Crack::Render(RenderContext* rc)
{

    model->render(rc->deviceContext, Transform, 0.f, color);
}

void Crack::Gui()
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
   
    }
    
}
