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
#include"scene_management.h"
#include"scene_title.h"
#include"scene_loading.h"
Goal::Goal(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    Gimic_type = Gimic_Type::Goal;
    Scale = { 0.5f,1.0f,0.5f };
    initialaize_Set_attribute(ObjType::null, ObjType::null);
}
Goal::Goal(ID3D11Device* device, const char* filename_)
{
    model = make_unique<Model>(device, filename_, true);
    Gimic_type = Gimic_Type::Goal;
    Scale = { 0.05f,0.2f,0.05f };
    initialaize_Set_attribute(ObjType::null, ObjType::null);
}
void Goal::Update(float elapsedTime)
{
    Goal_Flag = false;
    if (GoalInPosition())
    {
        Goal_Flag = true;
        //SceneManagement::instance().SceneChange(new SceneLoading(new SceneTitle));
    }
  
    UpdateTransform();
}

bool Goal::GoalInPosition()
{
    DebugRenderer& ince = DebugRenderer::incetance(Graphics::Instance().GetDevice());
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
    color = { 1,1,1,1 };

    DebugRenderer& ince_d = DebugRenderer::incetance(Graphics::Instance().GetDevice());
    ince_d.DrawSphere(Position, radius * radius, { 0,1,0,1 });
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
