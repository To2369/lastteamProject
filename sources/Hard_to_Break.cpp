#include "Hard_to_Break.h"

Hard_to_Break::Hard_to_Break(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::cution,ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Hard_to_Break::~Hard_to_Break()
{

}

void Hard_to_Break::Update(float elapsedTime)
{
    if (NotUpdateFlag)
    {
        Return_orijinal_ObjType(elapsedTime);
        ObjType_effect(elapsedTime);
        UpdateTransform();
        Velocty.y = 0;
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

Super_hard_to_Break::Super_hard_to_Break(ID3D11Device* device)
{
    Normal = { 0.f,-1.f,0.f };
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_cution,ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
}

Super_hard_to_Break::~Super_hard_to_Break()
{

}

void Super_hard_to_Break::Update(float elapsedTime)
{
    if (NotUpdateFlag)
    {
        Return_orijinal_ObjType(elapsedTime);
        ObjType_effect(elapsedTime);
        UpdateTransform();
        Velocty.y = 0;
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
