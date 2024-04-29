#include"Cution.h"
#include <Graphics/shader.h>
using namespace std;
#include"objectManajer.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include"PlayerManager.h"
#include"Graphics/DebugRenderer.h"
Cution::Cution(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 1.f;
}

Cution::~Cution()
{

}

void Cution::Update(float elapsedTime)
{
    Return_orijinal_ObjType(elapsedTime);
    VeloctyY = -elapsedTime*0.9f;
    PlayerManager& ince_pl = PlayerManager::Instance();
    Objectmanajer& ince_o = Objectmanajer::incetance();
    Player* pl = ince_pl.GetPlayer(0);
    XMFLOAT3 outpos;
    if (ince_o.Sphere_VS_Player(pl->GetPosition(), pl->getRadius(), this->Position, this->radius, outpos))
    {
        
        pl->SetPosition(outpos);
    }

    RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Cution::Render(RenderContext* rc)
{

    DebugRenderer& ince = DebugRenderer::incetance(rc->device);
    ince.DrawSphere(Position, radius, { 1,1,1,1 });
    model->render(rc->deviceContext, Transform, 0.0f, color);


}

void Cution::Gui()
{
    BaseGui();
    ImGui::SliderFloat("radius", &radius, 0.f, 1.0f);
}

Super_Cution::Super_Cution(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 1.f;

}

Super_Cution::~Super_Cution()
{
}

void Super_Cution::Update(float elapsedTime)
{
    Return_orijinal_ObjType(elapsedTime);
    VeloctyY = -elapsedTime*0.7f;
    PlayerManager& ince_pl = PlayerManager::Instance();
    Objectmanajer& ince_o = Objectmanajer::incetance();
    Player* pl = ince_pl.GetPlayer(0);
    XMFLOAT3 outpos;
    if (ince_o.Sphere_VS_Player(pl->GetPosition(), pl->getRadius(), this->Position, this->radius, outpos))
    {

        pl->SetPosition(outpos);
    }
    RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_Cution::Render(RenderContext* rc)
{
    DebugRenderer& ince_d = DebugRenderer::incetance(rc->device);
    ince_d.DrawSphere(Position, radius * radius, { 0,1,0,1 });
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Super_Cution::Gui()
{
    BaseGui();
    ImGui::SliderFloat("radius", &radius, 0.f, 1.0f);
}
