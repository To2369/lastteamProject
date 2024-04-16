#include"Gimic.h"
#include"objectManajer.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif

Switch::Switch(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device,filename,true);
    Scale = { 1.452999f,3.f, 0.930000f };
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
        
        if (obj->Get_isGimic_UpPosNow()&&obj->Get_BootGimicType()==this->GetGimicID())
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
    if (ImGui::CollapsingHeader("Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("position"))
            {

                XMFLOAT3 pos{ GetPosition() };
                ImGui::InputFloat("Position.x:", &pos.x);
                ImGui::InputFloat("Position.y:", &pos.y);
                ImGui::InputFloat("Position.z:", &pos.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("scale"))
            {
                XMFLOAT3 scale{ GetScale() };
                ImGui::InputFloat("scale.x:", &scale.x);
                ImGui::InputFloat("scale.y:", &scale.y);
                ImGui::InputFloat("scale.z:", &scale.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("normal"))
            {
                XMFLOAT3 normal{ Normal };
                ImGui::InputFloat("normal.x:", &normal.x);
                ImGui::InputFloat("normal.y:", &normal.y);
                ImGui::InputFloat("normal.z:", &normal.z);

                ImGui::TreePop();
            }
        }

        if (ImGui::CollapsingHeader("move_obj", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("move_position"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("move_Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                Position.x += pos.x;
                Position.y += pos.y;
                Position.z += pos.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_scale"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("move_scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                Scale.x += scale.x;
                Scale.y += scale.y;
                Scale.z += scale.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_normal"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 normal{ Normal };
                ImGui::SliderFloat("move_normal.x:", &normal.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_normal.y:", &normal.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_normal.z:", &normal.z, valu.Min.z, valu.Max.z);

                ImGui::TreePop();
            }
        }
        if (ImGui::TreeNode("Switch_Boot_Check"))
        {
            bool f = this->Get_SwitchFlag();
            ImGui::Checkbox("Switch_Boot",&f);
            ImGui::TreePop();
        }
    }
    const int buffer = 256;
    ImGui::InputText("ID", const_cast<char*>(ID.c_str()), buffer);
    ImGui::Text(ID.c_str());
}
