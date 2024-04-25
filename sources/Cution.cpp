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
    VeloctyY = -elapsedTime;
    //box_Collition_obj();

    if (!Get_isGimic_UpPosNow())RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Cution::Render(RenderContext* rc)
{

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
    Scale.x = Scale.y = Scale.z = 10.f;

}

Super_Cution::~Super_Cution()
{
}

void Super_Cution::Update(float elapsedTime)
{
    Return_orijinal_ObjType(elapsedTime);
    VeloctyY = -elapsedTime;
    //box_Collition_obj();

    if (!Get_isGimic_UpPosNow())RayCastGround();
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
