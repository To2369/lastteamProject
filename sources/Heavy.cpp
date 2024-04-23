#include"Heavy.h"
#include <objectManajer.h>

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include"Graphics/DebugRenderer.h"
using namespace std;
Heavy::Heavy(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::heavy, ObjType::null);

    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 1.f;
    moveobjectFlag = true;
}

Heavy::~Heavy()
{

}

void Heavy::Update(float elapsedTime)
{
    Return_orijinal_ObjType(elapsedTime);
    {
        SphereQuadPlacement spheres(Position);
        spheres.sphereLength = 0.367f;
        CreateQuadPlacement(spheres);

    }
    Velocty.y = -elapsedTime * 2;
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    {
        //ƒMƒ~ƒbƒN‚É‘Î‚µ‚Ä‰½‚©‚·‚éfor•¶
        for (int i = 0; i < count; i++)
        {
            Gimic* gimic = ince.Get_GameGimic(i);
            if (ince.Bounding_Box_vs_Bounding_Box(this, gimic, true, 0.045f))
            {
                if (Get_isGimic_UpPosNow())
                {
                    string g = gimic->GetGimicID();
                    Set_GimicType(g);
                    Velocty.y = 0;
                    break;
                }
            }
        }
        HitSphere();
    }
    RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Heavy::Render(RenderContext* rc)
{
    DebugRenderer& ince = DebugRenderer::incetance(rc->device);
    ince.DrawSphere(Position, radius, { 1,1,1,1 });
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Heavy::Gui()
{
    BaseGui();
    ImGui::SliderFloat("radius", &radius, 0.f, 1.f);
}

Super_Heavy::Super_Heavy(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_heavy, ObjType::null);

    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    moveobjectFlag = true;
}

Super_Heavy::~Super_Heavy()
{
}

void Super_Heavy::Update(float elapsedTime)
{
    Return_orijinal_ObjType(elapsedTime);
    {
        SphereQuadPlacement spheres(Position);
        spheres.sphereLength = 0.367f;
        CreateQuadPlacement(spheres);

    }
    Velocty.y = -elapsedTime * 4;
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    HitSphere();
    RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_Heavy::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

void Super_Heavy::Gui()
{

    BaseGui();
    ImGui::SliderFloat("radius", &radius, 0.f, 1.0f);
}
