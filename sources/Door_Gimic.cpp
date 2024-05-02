#include"Gimic.h"
#include"objectManajer.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
#include"Graphics/DebugRenderer.h"
Door::Door(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::Super_hard_to_Break, ObjType::null);
    vector<string>filenames;
    filenames.push_back(filename);
    model = make_unique<Model>(device, filename,filename, true);
    Scale = { 1,1,1 };
    Gimic_type = Gimic_Type::Door;
    MyGimicFlagType = Gimic_Type::Switch;
    UpdateTransform();
}

void Door::Update(float elapsedTime)
{
    elapsedtime_ = elapsedTime;
    color = InitColor();
    ObjType_effect(elapsedTime);
    Gimic_effect(Gimic_Type::Door);
    Return_orijinal_ObjType(elapsedTime);
    FallDown(90);
    UpdateTransform();
}

void Door::Render(RenderContext* rc)
{
    DebugRenderer& ince_debug = DebugRenderer::incetance(rc->device);
  
    model->render(rc->deviceContext, Transform,elapsedtime_, color);

   /* for (auto& node : model->kefreame->nodes)
    {
        XMMATRIX S = XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z);
        XMMATRIX T= XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z);
        XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation));

        XMMATRIX world = XMMatrixMultiply(S*R*T,XMLoadFloat4x4(&Transform));
        XMFLOAT4X4 worldtransform;
        XMStoreFloat4x4(&worldtransform, world);
        XMFLOAT3 pos = {worldtransform._41,worldtransform._42,worldtransform._43};
        ince_debug.DrawSphere(pos, 0.05f, { 1,1,1,1 });
      
    }*/

}

void Door::Gimic_VS_GimicFlagBoot()
{
#if 1
    this->SetBootFlag(false);
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
       
        bool f = GetBootFlag();
        ImGui::Checkbox("Door_Boot", &f);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Angle"))
    {
        XMFLOAT3 angle{};
        ImGui::SliderFloat3("angle", &angle.x, -0.01f, 0.01f);
        Angle.x += angle.x;
        Angle.y += angle.y;
        Angle.z += angle.z;
        ImGui::TreePop();
    }
    const int buffer = 256;
    ImGui::InputText("ID", const_cast<char*>(ID.c_str()), buffer);
    ImGui::Text(ID.c_str());

    static bool stop = false;
    ImGui::Checkbox("stopAnimation", &stop);
    model->stop_animation = stop;
}
