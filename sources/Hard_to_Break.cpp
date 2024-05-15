#include "Hard_to_Break.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif

Hard_to_Break::Hard_to_Break(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    moveobjectFlag = true;
}

Hard_to_Break::Hard_to_Break(ID3D11Device* device, const char* filename_)
{
    model = make_unique<Model>(device, filename_, true);
    initialaize_Set_attribute(ObjType::cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    moveobjectFlag = true;
}

Hard_to_Break::~Hard_to_Break()
{

}

void Hard_to_Break::Update(float elapsedTime)
{
    if (GetStatic_Objflag())
    {
        UpdateTransform();
        return;
    }
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
    BaseGui();
    ImGui::SliderFloat("radius", &radius, 0.f, 1.0f);
}

Super_hard_to_Break::Super_hard_to_Break(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    moveobjectFlag = true;
}

Super_hard_to_Break::Super_hard_to_Break(ID3D11Device* device, const char* filename_)
{
    model = make_unique<Model>(device, filename_, true);
    initialaize_Set_attribute(ObjType::Super_cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    moveobjectFlag = true;
}

Super_hard_to_Break::~Super_hard_to_Break()
{

}

void Super_hard_to_Break::Update(float elapsedTime)
{
    if (GetStatic_Objflag())
    {
        UpdateTransform();
        return;
    }
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

    BaseGui();
    ImGui::SliderFloat("radius", &radius, 0.f, 1.0f);
}
