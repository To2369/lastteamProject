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
#endif
#include"objectManajer.h"
#include"Graphics/DebugRenderer.h"
#include"PlayerManager.h"
#include"variable_management_class_for_hit_test.h"
#include"Collision.h"
DropBox_Road::DropBox_Road(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    Baria_Wall = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    Gimic_type = Gimic_Type::Drop_Road;
    Scale = { 10.f,10.f,10.f };
    color = { 1.f,1.f,1.f,1.f };

}
void DropBox_Road::Update(float elapsedTime)
{

    Objectmanajer& ince_o = Objectmanajer::incetance();
    Camera& ince_c = Camera::instance();
    dropbox.flag = false;
    dropbox.oppnentPos = {};
    int count = ince_o.Get_GameObjCount();
    {
        for (int i = 0; i < count; i++)
        {
            Object* obj = ince_o.Get_GameObject(i);
            if (ince_o.Bounding_Box_vs_Bounding_Box(obj, this, true, 0.045f))
            {
                if (obj->Get_isGimic_UpPosNow())
                {
                  
                    dropbox.oppnentPos = obj->GetPosition();
                    dropbox.radius = obj->GetRadius();
                    break;
                }
            }
           
        }
      

    }

    UpdateTransform();
    IsWall_UpdateTransform();
}
void DropBox_Road::Render(RenderContext* rc)
{
    DebugRenderer& ince_d = DebugRenderer::incetance(rc->device);
    ince_d.DrawSphere(dropbox.oppnentPos, dropbox.radius, { 0,1,0,1 });
    model->render(rc->deviceContext, Transform, 0.f, color);
    Baria_Wall->render(rc->deviceContext, BoxTransform, 0.f, {1.f, 0.f, 0.f, 0.3f});
}


void DropBox_Road::Gui()
{
    if (ImGui::CollapsingHeader("Wall_Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Wall_Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("Wall_position"))
            {

                XMFLOAT3 pos{BoxPosition};
                ImGui::InputFloat("Wall_Position.x:", &pos.x);
                ImGui::InputFloat("Wall_Position.y:", &pos.y);
                ImGui::InputFloat("Wall_Position.z:", &pos.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Wall_scale"))
            {
                XMFLOAT3 scale{BoxScale};
                ImGui::InputFloat("Wall_scale.x:", &scale.x);
                ImGui::InputFloat("Wall_scale.y:", &scale.y);
                ImGui::InputFloat("Wall_scale.z:", &scale.z);

                ImGui::TreePop();
            }

        }

        if (ImGui::CollapsingHeader("move_Wall_obj", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("move_Wall_position"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("move_Wall_Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_Wall_Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_Wall_Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                BoxPosition.x += pos.x;
                BoxPosition.y += pos.y;
                BoxPosition.z += pos.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_Wall_scale"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("move_Wall_scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_Wall_scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_Wall_scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                BoxScale.x += scale.x;
                BoxScale.y += scale.y;
                BoxScale.z += scale.z;
                ImGui::TreePop();
            }

        }
    }
    BaseGui();

   
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


bool DropBox_Road::isPlayerInRangeOf_Box()
{

   
    PlayerManager& ince_p = PlayerManager::Instance();
    Player* pl = ince_p.GetPlayer(0);
    XMFLOAT2 PLpos{pl->GetPosition().x,pl->GetPosition().z};
    XMFLOAT2 boxpos = { dropbox.oppnentPos.x,dropbox.oppnentPos.z };
    XMVECTOR Len = XMVectorSubtract(XMLoadFloat2(&boxpos), XMLoadFloat2(&PLpos));
    float len = XMVectorGetX(XMVector3LengthSq(Len));
    float Radius = dropbox.radius+pl->getRadius();
    if (len < Radius * Radius)
    {
        dropbox.flag = true;
        return true;
    }

    return false;
}

void DropBox_Road::IsWall_UpdateTransform()
{
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
      { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
      { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
      { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
      { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    const float scale_factor = 0.01f;
    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };

    //スケール行列を作成
    XMMATRIX S = XMMatrixScaling(BoxScale.x, BoxScale.y, BoxScale.z);
    //回転行列作成
   
    XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(BoxAngle.x, BoxAngle.y, BoxAngle.z);
    //位置行列をBoxPosition
    XMMATRIX T = XMMatrixTranslation(BoxPosition.x, BoxPosition.y, BoxPosition.z);
    //4つの行列を組み合わせて、ワールド座標を作成
    DirectX::XMMATRIX W = C * S * R * T;
    //計算したワールド座標を取り出す
    DirectX::XMStoreFloat4x4(&BoxTransform, W);

}
