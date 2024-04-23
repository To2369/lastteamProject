#include"Gimic.h"
#include"objectManajer.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
Door::Door(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::Super_hard_to_Break, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    Scale = { 1,1,1 };
    Gimic_type = Gimic_Type::Door;
    MyGimicFlagType = Gimic_Type::Switch;
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
#if 1
    bootFlag.clear();
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        Gimic* gimic = ince.Get_GameGimic(i);
        if (this->Get_GimicType() == gimic->Get_GimicType())continue;
        if (this->GetGimicID() != gimic->GetGimicID())continue;
        if (gimic->Get_GimicType() == this->GetMyGimicFlagType() && gimic->Get_SwitchFlag())
        {
            this->SetBootFlag(true);
        }

    }
#endif

}

void Door::Gui()
{
    BaseGui();
    if (ImGui::TreeNode("Door_BootChack"))
    {
        int count = bootFlag.size();
        for (int i = 0; i < count; i++)
        {
            bool f = GetBootFlag(i);
            ImGui::Checkbox("Door_Boot", &f);
        }
        ImGui::TreePop();
    }

    const int buffer = 256;
    ImGui::InputText("ID", const_cast<char*>(ID.c_str()), buffer);
    ImGui::Text(ID.c_str());
}
