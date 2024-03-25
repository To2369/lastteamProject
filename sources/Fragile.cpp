#include "Fragile.h"

Fragile::Fragile(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Fragile,ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Fragile::~Fragile()
{
}

void Fragile::Update(float elapsedTime)
{
 
    color = InitColor();
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (!Get_isGimic_UpPosNow())RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}


void Fragile::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}

Super_fragile::Super_fragile(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_fragile, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Super_fragile::~Super_fragile()
{

}

void Super_fragile::Update(float elapsedTime)
{
   
    color = InitColor();
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (!Get_isGimic_UpPosNow())RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_fragile::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
}
