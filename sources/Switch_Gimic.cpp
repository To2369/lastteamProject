#include"Gimic.h"
#include"objectManajer.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif

#include "Graphics/graphics.h"

Switch::Switch(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    Scale = { 1.452999f,3.456001f, 0.930000f };
    Gimic_type = Gimic_Type::Switch;
    MyObjeFlagType = ObjType::heavy;
}

Switch::Switch(ID3D11Device* device, const char* filename_)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device, filename_, true);
    Scale = { 1.452999f,3.456001f, 0.930000f };
    Gimic_type = Gimic_Type::Switch;
    MyObjeFlagType = ObjType::heavy;
}

void Switch::Update(float elapsedTime)
{

    color = InitColor();
    Gimic_effect(Gimic_Type::Switch);
    UpdateTransform();
}

void Switch::Render(RenderContext* rc)
{
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0.0f, color);
}

bool Switch::gimic_VS_Object()
{
    //raycastでのギミック起動
#if false
    get_gameobj = ObjType::null;
    VMCFHT& ince_rc = VMCFHT::instance();
    Objectmanajer& ince_ob = Objectmanajer::incetance();

    XMFLOAT3 raynormal = GetNormal();
    XMFLOAT3 pos = Position;

    pos.y += -0.05f;
    ince_rc.update(pos, raynormal);
    float Length = 0.5f;
    int count = ince_ob.Get_GameObjCount();
    vector<ObjType>type;
    for (int i = 0; i < count; i++)
    {
        Object* obj = ince_ob.Get_GameObject(i);
        collision_mesh* mesh = obj->GetModel()->Get_RaycastCollition();
        if (ince_rc.raycast(*mesh, obj->GetTransform(), result_intersection, Length))
        {
            type.push_back(obj->Get_attribute());
        }
    }
    count = type.size();
    if (count == 0)return false;
    for (int i = 0; i < count; i++)
    {
        if (MyGimicFlagType != type[i])
        {
            return false;
        }
    }

    get_gameobj = MyGimicFlagType;
    type.clear();//
#endif
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameObjCount();
    vector<ObjType>type;

    for (int i = 0; i < count; i++)
    {
        Object* obj = ince.Get_GameObject(i);

        if (obj->Get_isGimic_UpPosNow() && obj->Get_BootGimicType() == this->GetGimicID())
        {
            type.push_back(obj->Get_Old_Objtype(0));
        }

    }

    count = type.size();
    if (count == 0 || count > 1)return false;
    for (int i = 0; i < count; i++)
    {
        if (MyObjeFlagType != type[i])
        {
            return false;
        }
    }

    type.clear();
    return true;
}

void Switch::Gui()
{
    BaseGui();
    //map<key,取り出す型>
    std::map<ObjType, string>enumClassToString
    {
        {ObjType::heavy,"heavy"},
        {ObjType::Super_heavy,"super_heavy"},
        {ObjType::cution,"cution"},
        {ObjType::Super_cution,"super_cution"},
        {ObjType::Hard_to_Break,"Hard_to_Break"},
        {ObjType::Super_hard_to_Break,"Super_Hard_to_Break"},
        {ObjType::Fragile,"Fragile"},
        {ObjType::Super_fragile,"Super_Fragile"},
        {ObjType::Crack,"Crack"},

    };
    

    if (ImGui::TreeNode("Switch_Boot_Check"))
    {
        bool f = this->Get_SwitchFlag();
        ImGui::Checkbox("Switch_Boot", &f);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("MyobjectTypeFlagSet"))
    {
        if (ImGui::Button("heavy")) MyObjeFlagType = ObjType::heavy;
        if (ImGui::Button("Super_heavy")) MyObjeFlagType = ObjType::Super_heavy;
        if (ImGui::Button("Cution")) MyObjeFlagType = ObjType::cution;
        if (ImGui::Button("Super_cution")) MyObjeFlagType = ObjType::Super_cution;
        if (ImGui::Button("Hard_to_Break")) MyObjeFlagType = ObjType::Hard_to_Break;
        if (ImGui::Button("Super_hard_to_Break")) MyObjeFlagType = ObjType::Super_hard_to_Break;
        if (ImGui::Button("Fragile")) MyObjeFlagType = ObjType::Fragile;
        if (ImGui::Button("Super_fragile")) MyObjeFlagType = ObjType::Super_fragile;
        if (ImGui::Button("Crack")) MyObjeFlagType = ObjType::Crack;
        ImGui::TreePop();
    }
    ImGui::Text(enumClassToString[MyObjeFlagType].c_str());
    const int buffer = 256;
    ImGui::InputText("ID", const_cast<char*>(ID.c_str()), buffer);
    ImGui::Text(ID.c_str());
}
