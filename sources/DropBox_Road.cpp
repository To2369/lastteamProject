#include"Gimic.h"
#include"objectManajer.h"
#include"camera.h"
using namespace std;
using namespace DirectX;

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
DropBox_Road::DropBox_Road(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename,true);
    HitBox = make_unique<Model>(device, filename,true);
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    Gimic_type = Gimic_Type::Drop_Road;
    Scale = { 10.f,10.f,10.f };
    color = {1.f,1.f,1.f,1.f};
  
}
void DropBox_Road::Update(float elapsedTime)
{
  
    Objectmanajer& ince_o = Objectmanajer::incetance();
    Camera& ince_c = Camera::instance();
    int count = ince_o.Get_GameObjCount();
    {
        for (int i = 0; i < count; i++)
        {
            Object* obj = ince_o.Get_GameObject(i);
            if (ince_o.Bounding_Box_vs_Bounding_Box(obj, this, true, 0.045f))
            {
                if (obj->Get_isGimic_UpPosNow())
                {
                    playerStopFlag = true;
                    dropbox.oppnentPos = obj->GetPosition();
                    dropbox.flag = true;
                    dropbox.scaleX = obj->GetScale().x;
                    
                }
            }
            else
            {
               
                dropbox.oppnentPos = {};
                dropbox.flag = false;
            }
        }
        if (dropbox.flag&&isPlayerInRangeOf_Box())
        {
            playerStopFlag = false;
        }
        else playerStopFlag = true;
    }
 
    UpdateTransform();
    HitBox_TransformUpdate();
}
void DropBox_Road::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.f, color);
    HitBox->render(rc->deviceContext, HitBox_Transform, 0.f, {1,1,1,0.3f});
}
//ç°ì˙ÇÕÇ±Ç±Ç‹Ç≈

void DropBox_Road::Gui()
{

    if (ImGui::CollapsingHeader("Road_Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("_position"))
            {

                XMFLOAT3 pos{ GetPosition() };
                ImGui::InputFloat("Position.x:", &pos.x);
                ImGui::InputFloat("Position.y:", &pos.y);
                ImGui::InputFloat("Position.z:", &pos.z);
                
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("_scale"))
            {
                XMFLOAT3 scale{ GetScale() };
                ImGui::InputFloat("scale.x:", &scale.x);
                ImGui::InputFloat("scale.y:", &scale.y);
                ImGui::InputFloat("scale.z:", &scale.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("_normal"))
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
            if (ImGui::TreeNode("position"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                Position.x += pos.x;
                Position.y += pos.y;
                Position.z += pos.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("scale"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                Scale.x += scale.x;
                Scale.y += scale.y;
                Scale.z += scale.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("normal"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 normal{ Normal };
                ImGui::SliderFloat("normal.x:", &normal.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("normal.y:", &normal.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("normal.z:", &normal.z, valu.Min.z, valu.Max.z);

                ImGui::TreePop();
            }
        }
       
    }
    if (ImGui::CollapsingHeader("Box_Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Box_Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("_Box_position"))
            {

                XMFLOAT3 pos{ GetHitBox_Position() };
                ImGui::InputFloat("_Box_Position.x:", &pos.x);
                ImGui::InputFloat("_Box_Position.y:", &pos.y);
                ImGui::InputFloat("_Box_Position.z:", &pos.z);
              
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("_Box_scale"))
            {
                XMFLOAT3 scale{ GetHitBox_Scale() };
                ImGui::InputFloat("_Box_scale.x:", &scale.x);
                ImGui::InputFloat("_Box_scale.y:", &scale.y);
                ImGui::InputFloat("_Box_scale.z:", &scale.z);

                ImGui::TreePop();
            }
        }
        
        if (ImGui::CollapsingHeader("Box_move_obj", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("Box_position"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("Box_Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("Box_Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("Box_Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                HitBox_Position.x += pos.x;
                HitBox_Position.y += pos.y;
                HitBox_Position.z += pos.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Box_scale"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("Box_scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("Box_scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("Box_scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                HitBox_Scale.x += scale.x;
                HitBox_Scale.y += scale.y;
                HitBox_Scale.z += scale.z;
                ImGui::TreePop();
            }
          
        }
      
    }
    if (ImGui::CollapsingHeader("Flags", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("isPlayerInRangeOf_Box")) {
            bool f = isPlayerInRangeOf_Box();
            ImGui::Checkbox("", &f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("GetPlayerStopFlag")) {
            bool f = GetPlayerStopFlag();
            ImGui::Checkbox("", &f);
            ImGui::TreePop();
        }
    }
   
    ImGui::Text(ID.c_str());
}



void DropBox_Road::HitBox_TransformUpdate()
{
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
     { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
     { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
     { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
     { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    const float scale_factor = 0.01f;
    XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
    XMMATRIX S{ XMMatrixScaling(HitBox_Scale.x
                               ,HitBox_Scale.y,
                                HitBox_Scale.z) };
    XMMATRIX R{XMMatrixRotationRollPitchYaw(HitBox_Angle.x,HitBox_Angle.y,HitBox_Angle.z)};
    XMMATRIX T{ XMMatrixTranslation(HitBox_Position.x
                                   ,HitBox_Position.y,
                                    HitBox_Position.z) };

    XMMATRIX World=C*S*R*T;
    XMStoreFloat4x4(&HitBox_Transform, World);
   

}
bool DropBox_Road::isPlayerInRangeOf_Box()
{
    Camera& ince_c = Camera::instance();
    XMFLOAT2 camerapos = { ince_c.GetEye().x,ince_c.GetEye().z };
    XMFLOAT2 boxpos = { dropbox.oppnentPos.x,dropbox.oppnentPos.z};
    XMVECTOR Len =XMVectorSubtract(XMLoadFloat2(&camerapos),XMLoadFloat2(&boxpos));
    float len=XMVectorGetX(XMVector3Length(Len));
   
    float radius =dropbox.scaleX*0.1f;
    if (len < radius)
    {
        return true;
    }
    return false;
}