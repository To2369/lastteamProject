#include "InvisibleBarria.h"
using namespace std;
using namespace DirectX;
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include "Graphics/graphics.h"
#include"objectManajer.h"
InvisibleBarrier::InvisibleBarrier(ID3D11Device* device,XMFLOAT3 Scale_)
{
    model = make_unique<Model>(device, filename, true);
    type = Static_ObjType::Invisible_Wall;
    Scale = Scale_;
    //Scale = { 291.298f,198.939f,20.385 };
}

InvisibleBarrier::InvisibleBarrier(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    type = Static_ObjType::Invisible_Wall;
    Scale = { 291.298f,198.939f,20.385 };
}

void InvisibleBarrier::Update(float elapsedTime)
{
    //âûã}èàíu
    
    if (id != "")
    {
        Objectmanajer& ince = Objectmanajer::incetance();
        int count = ince.Get_GameGimicCount();
        for (int i = 0; i < count; i++)
        {
            Gimic* gimic = ince.Get_GameGimic(i);
            if (gimic->GetGimicID() == id&&gimic->GetBootFlag()&&Flagtype==gimic->Get_GimicType())
            {
                this->Destroy();
            }
        }
    }
    UpdateTransform();
}

void InvisibleBarrier::Render(RenderContext* rc)
{
    Graphics& graphics = Graphics::Instance();
  /*  graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(1), nullptr, 0xFFFFFFFF);
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0, {0.5f,0.5f,0.5f,0.1f});

    graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);*/
}

void InvisibleBarrier::Gui()
{
    BaseGui();
    //map<key,éÊÇËèoÇ∑å^>
    std::map<Gimic_Type, string>enumClassToString
    {
        {Gimic_Type::Door,"Door"},
        {Gimic_Type::Drop_Road,"Drop_Road"},
        {Gimic_Type::Goal,"Goal"},
        {Gimic_Type::Lift,"Lift"},
        {Gimic_Type::Move_Object,"Move_Object"},
        {Gimic_Type::Switch,"Switch"},
        {Gimic_Type::null,"null"}

    };
    if (ImGui::TreeNode("MyobjectTypeFlagSet"))
    {
        if (ImGui::Button("Door")) Flagtype = Gimic_Type::Door;
        if (ImGui::Button("Drop_Road")) Flagtype = Gimic_Type::Drop_Road;
        if (ImGui::Button("Goal")) Flagtype = Gimic_Type::Goal;
        if (ImGui::Button("Lift")) Flagtype = Gimic_Type::Lift;
        if (ImGui::Button("Move_Object")) Flagtype = Gimic_Type::Move_Object;
        if (ImGui::Button("Switch")) Flagtype = Gimic_Type::Switch;
        if (ImGui::Button("null")) Flagtype =Gimic_Type::null;
       
        ImGui::TreePop();
    }
    ImGui::Text(enumClassToString[Flagtype].c_str());
    ImGui::Text(id.c_str());
    
}
