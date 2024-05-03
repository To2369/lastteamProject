#include"Heavy.h"
#include <objectManajer.h>
#include "Graphics/graphics.h"

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
    oldPosition = Position;
    Return_orijinal_ObjType(elapsedTime);
    {
        SphereQuadPlacement spheres(Position);
        spheres.sphereLength = 0.367f;
        CreateQuadPlacement(spheres);

    }
    VeloctyY = -elapsedTime * 2;
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
                    VeloctyY = 0;
                    break;
                }
            }
        }
       
        if (!GetIsWall()&&!GetIsObject())
        {
            if (VelocityXZ.x > 0.f || VelocityXZ.x < 0 || VelocityXZ.y>0 || VelocityXZ.y < 0)
            {
                Position.x += VelocityXZ.x;
                Position.z += VelocityXZ.y;
            }
        }
    }
    InvisibleWall_VS_Object();
    isObject = false;
    count = ince.Get_GameObjCount();
    for (int i = 0; i < count; i++)
    {
        Object* obj = ince.Get_GameObject(i);
        if (this == obj)continue;
        if (ince.Bounding_Box_vs_Bounding_Box(this, obj, false))
        {
            Position = oldPosition;
            isObject = true;
            break;
        }
    }

    RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Heavy::Render(RenderContext* rc)
{
    DebugRenderer& ince = DebugRenderer::incetance(Graphics::Instance().GetDevice());
    ince.DrawSphere(Position, radius, { 1,1,1,1 });
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0.0f, color);
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
    VeloctyY = -elapsedTime * 4;
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
   
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
