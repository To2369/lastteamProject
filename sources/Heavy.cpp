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
#include"variable_management_class_for_hit_test.h"

using namespace std;
Heavy::Heavy(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::heavy, ObjType::null);
    spehereLength = 0.362;
    spehereRadius = 0.030;
    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 1.f;
    moveobjectFlag = true;

}

Heavy::Heavy(ID3D11Device* device, const char* filename_)
{
    model = make_unique<Model>(device, filename_, true);
    initialaize_Set_attribute(ObjType::heavy, ObjType::null);
    SphereQuadPlacement spheres;
    spehereLength = spheres.sphereLength;
    spehereRadius = spheres.SphereRadius;
    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 1.f;
    moveobjectFlag = true;
}

Heavy::~Heavy()
{

}

void Heavy::Update(float elapsedTime)
{
 /*   if (GetStatic_Objflag())
    {
        UpdateTransform();
        return;
    }*/
    oldPosition = Position;

    Return_orijinal_ObjType(elapsedTime);
    {
        CreateSphere();
    }
    VeloctyY = -elapsedTime * 2;
    Objectmanajer& ince_o = Objectmanajer::incetance();
    VMCFHT& ince_ray = VMCFHT::instance();
    
    

    isObject = false;
    int count = ince_o.Get_GameObjCount();

    {
        if (israycast.IsRayCastObject)
        {
            for (int i = 0; i < count; i++)
            {
                Object* obj = ince_o.Get_GameObject(i);
                if (this == obj)continue;
                if (ince_o.Bounding_Box_vs_Bounding_Box(this, obj, false))
                {
                    Position = oldPosition;
                    isObject = true;
                    break;
                }
            }
        }
    }
    count = ince_o.Get_GameGimicCount();

    {
        if (israycast.IsBoundhingBoxVSGimic)
        {
            //ギミックに対して何かするfor文
            for (int i = 0; i < count; i++)
            {
                Gimic* gimic = ince_o.Get_GameGimic(i);
                if (gimic->Get_GimicType() == Gimic_Type::Lift)continue;
                if (ince_o.Bounding_Box_vs_Bounding_Box(this, gimic, true, 0.045f))
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
        }

    }
    if (!GetIsWall() && !GetIsObject())
    {
        if (VelocityXZ.x > 0.f || VelocityXZ.x < 0 || VelocityXZ.y>0 || VelocityXZ.y < 0)
        {
            Position.x += VelocityXZ.x;
            Position.z += VelocityXZ.y;
        }
    }
    if (israycast.IsRayCastInvisibleWall)
        InvisibleWall_VS_Object();
    {
        XMFLOAT3 start = Position;
        XMFLOAT3 end = Position;
        end.y -= 0.1f;
        HitResult hit;
        count = ince_o.Get_GameGimicCount();
        for (int i = 0; i < count; i++)
        {
            Gimic* obj = ince_o.Get_GameGimic(i);

            if (obj->GetLiftType() == Gimic::LiftType::null)continue;
            if (obj->Raycast(start, end, hit))
            {
                VeloctyY = 0;
                obj->SetisLift(this->Get_Old_Objtype(0));
                Position.y = hit.position.y + 0.1f;
                israycast.IsRayCastGround = false;
                break;
            }

        }

    }
    if (israycast.IsRayCastGround)
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
    Scale.x = Scale.y = Scale.z = 1.f;
    moveobjectFlag = true;
}

Super_Heavy::Super_Heavy(ID3D11Device* device, const char* filename_)
{
    model = make_unique<Model>(device, filename_, true);
    initialaize_Set_attribute(ObjType::Super_heavy, ObjType::null);
    spehereLength = 0.367f;
    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 1.f;
    moveobjectFlag = true;
}

Super_Heavy::~Super_Heavy()
{
}

void Super_Heavy::Update(float elapsedTime)
{

    /*if (GetStatic_Objflag())
    {
        UpdateTransform();
        return;
    }*/
    oldPosition = Position;

    Return_orijinal_ObjType(elapsedTime);
    {
        CreateSphere();

    }
    VeloctyY = -elapsedTime * 2;
    Objectmanajer& ince_o = Objectmanajer::incetance();
    VMCFHT& ince_ray = VMCFHT::instance();
    int count = ince_o.Get_GameGimicCount();

    {
        if (israycast.IsBoundhingBoxVSGimic)
        {
            //ギミックに対して何かするfor文
            for (int i = 0; i < count; i++)
            {
                Gimic* gimic = ince_o.Get_GameGimic(i);
                if (gimic->Get_GimicType() == Gimic_Type::Lift)continue;
                if (ince_o.Bounding_Box_vs_Bounding_Box(this, gimic, true, 0.045f))
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
        }

    }
    if (!GetIsWall() && !GetIsObject())
    {
        if (VelocityXZ.x > 0.f || VelocityXZ.x < 0 || VelocityXZ.y>0 || VelocityXZ.y < 0)
        {
            Position.x += VelocityXZ.x;
            Position.z += VelocityXZ.y;
        }
    }
    if (israycast.IsRayCastInvisibleWall)
        InvisibleWall_VS_Object();

    isObject = false;
    count = ince_o.Get_GameObjCount();

    {
        if (israycast.IsRayCastObject)
        {
            for (int i = 0; i < count; i++)
            {
                Object* obj = ince_o.Get_GameObject(i);
                if (this == obj)continue;
                if (ince_o.Bounding_Box_vs_Bounding_Box(this, obj, false))
                {
                    Position = oldPosition;
                    isObject = true;
                    break;
                }
            }
        }
    }


    {
        XMFLOAT3 start = Position;
        XMFLOAT3 end = Position;
        end.y -= 0.1f;
        HitResult hit;
        count = ince_o.Get_GameGimicCount();
        for (int i = 0; i < count; i++)
        {
            Gimic* obj = ince_o.Get_GameGimic(i);

            if (obj->GetLiftType() == Gimic::LiftType::null)continue;
            if (obj->Raycast(start, end, hit))
            {
                VeloctyY = 0;
                obj->SetisLift(this->Get_Old_Objtype(0));
                Position.y = hit.position.y + 0.1f;
                israycast.IsRayCastGround = false;
                break;
            }

        }

    }
    if (israycast.IsRayCastGround)
        RayCastGround();

    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_Heavy::Render(RenderContext* rc)
{
    DebugRenderer& ince = DebugRenderer::incetance(Graphics::Instance().GetDevice());
    ince.DrawSphere(Position, radius, { 1,1,1,1 });
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0.0f, color);
}

void Super_Heavy::Gui()
{

    BaseGui();

    ImGui::SliderFloat("radius", &radius, 0.f, 1.0f);
}
