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
#include"PlayerManager.h"
#include "Graphics/graphics.h"

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
    PlayerManager& ince_pl = PlayerManager::Instance();
    Player* pl = ince_pl.GetPlayer(0);
    XMVECTOR plPos = XMLoadFloat3(&pl->GetPosition());
    XMVECTOR GoalPos = XMLoadFloat3(&Position);

    XMVECTOR pos = XMVectorSubtract(GoalPos, plPos);
    float len = XMVectorGetX(XMVector3Length(pos));
    if (len < radius + pl->getRadius())
    {
        return true;
    }
    ince.DrawSphere(Position, radius, { 0,1,0,1 });
    return false;
}

void Goal::Render(RenderContext* rc)
{
    color = { 1,0,0,1 };
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0.0f, color);
}

void Goal::Gui()
{


    BaseGui();
    if (ImGui::TreeNode("GoalRadius"))
    {
        ImGui::SliderFloat("radius:", &radius, 0.1f, 1.0f);
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
    ImGui::Text(ID.c_str());
}
