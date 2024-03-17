#include"Gimic.h"
#include"objectManajer.h"
Door::Door(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::Super_hard_to_Break, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    Scale = { 0.05f,0.5f,0.05f };
    Gimic_type = Gimic_Type::Door;
    MyGimicFlagType = Gimic_Type::Switch;
    Normal = { 0.f,1.f,0.f };
    UpdateTransform();
}

void Door::Update(float elapsedTime)
{
    color = InitColor();
    ObjType_effect(elapsedTime);
    Gimic_effect(Gimic_Type::Door);
    Return_orijinal_ObjType(elapsedTime);
    UpdateTransform();
}

void Door::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
  
}

void Door::Gimic_VS_GimicFlagBoot()
{
    bootFlag.clear();
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        Gimic* gimic = ince.Get_GameGimic(i);
        if (this->Get_GimicType() == gimic->Get_GimicType())continue;
        if (gimic->Get_GimicType() == this->GetMyGimicFlagType() && gimic->Get_SwitchFlag())
        {
            this->SetBootFlag(true);
        }

    }
}
