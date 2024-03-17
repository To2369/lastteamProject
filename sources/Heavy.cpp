#include"Heavy.h"
using namespace std;
Heavy::Heavy(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::heavy, ObjType::null);

    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Heavy::~Heavy()
{

}

void Heavy::Update(float elapsedTime)
{
    if (NotUpdateFlag)
    {
        Return_orijinal_ObjType(elapsedTime);
        ObjType_effect(elapsedTime);
        UpdateTransform();
        Velocty.y = 0;
        return;
    }
    
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (isGimic_UpPosNow)Velocty.y = 0.f;
    else if (!isGimic_UpPosNow)RayCastGround();

    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Heavy::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

Super_Heavy::Super_Heavy(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_heavy, ObjType::null);

    //Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Super_Heavy::~Super_Heavy()
{
}

void Super_Heavy::Update(float elapsedTime)
{
    if (NotUpdateFlag)
    {
        Return_orijinal_ObjType(elapsedTime);
        ObjType_effect(elapsedTime);
        UpdateTransform();
        Velocty.y = 0;
        return;
    }
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (isGimic_UpPosNow)Velocty.y = 0.f;
    else if (!isGimic_UpPosNow)RayCastGround();

    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_Heavy::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}
