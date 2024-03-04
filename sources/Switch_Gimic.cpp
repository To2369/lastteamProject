#include"Gimic.h"



Switch::Switch(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device,filename,true);
    Scale = { 0.05f,0.05f,0.05f };
    Gimic_type = Gimic_Type::Switch;
    MyObjeFlagType = ObjType::heavy;
    Normal = { 0.f,1.f,0.f };
}

void Switch::Update(float elapsedTime)
{
   
    color = InitColor();
    Gimic_effect(Gimic_Type::Switch);
    UpdateTransform();
}

void Switch::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
    
}

