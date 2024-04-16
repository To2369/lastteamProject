#include"Gimic.h"
#include"camera.h"
#include"Graphics/DebugRenderer.h"
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
Goal::Goal(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    Gimic_type = Gimic_Type::Goal;
    Scale = { 0.05f,0.2f,0.05f };
    initialaize_Set_attribute(ObjType::null, ObjType::null);
   
    device = device;
}
void Goal::Update(float elapsedTime)
{
    Goal_Flag = false;
    if (GoalInPosition())
    {
        Goal_Flag = true;
    }
    UpdateTransform();
}

bool Goal::GoalInPosition()
{
    DebugRenderer& ince = DebugRenderer::incetance(device);
    Camera& ince_c = Camera::instance();
    XMVECTOR cameraPos = XMLoadFloat3(&ince_c.GetEye());
    XMVECTOR GoalPos = XMLoadFloat3(&Position);

    XMVECTOR pos = XMVectorSubtract(GoalPos, cameraPos);
    float len = XMVectorGetX(XMVector3Length(pos));
    if (len < radius + radius)
    {
        return true;
    }
    ince.DrawSphere(Position, radius,{0,1,0,1});
    return false;
}

void Goal::Render(RenderContext* rc)
{
    color = { 1,0,0,1 };
    model->render(rc->deviceContext, Transform, 0.0f, color);

}

void Goal::Gui()
{
   
    
    if (ImGui::CollapsingHeader("Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("Goal"))
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

            if (ImGui::TreeNode("GoalRadius"))
            {
                ImGui::SliderFloat("scale.x:", &radius, 0.1f, 1.0f);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Goal"))
            {
                bool f = GoalInPosition();
                ImGui::Checkbox("GoalInPosition", &f);
                f = Get_GoalFlag();
                ImGui::Checkbox("Get_GoalFlag", &f);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    ImGui::Text(ID.c_str());
}
