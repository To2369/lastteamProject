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
DropBox_Road::DropBox_Road(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    Gimic_type = Gimic_Type::Drop_Road;
    Scale = { 10.f,10.f,10.f };
    color = { 1.f,1.f,1.f,1.f };

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
                    dropbox.radius = obj->GetRadius();
                }
            }
            else
            {

                dropbox.oppnentPos = {};
                dropbox.flag = false;
            }
        }
        if (dropbox.flag && isPlayerInRangeOf_Box())
        {
            playerStopFlag = false;
        }
        else playerStopFlag = true;
    }

    UpdateTransform();
}
void DropBox_Road::Render(RenderContext* rc)
{
    DebugRenderer& ince_d = DebugRenderer::incetance(rc->device);
    ince_d.DrawSphere(dropbox.oppnentPos, radius * radius, { 0,1,0,1 });
    model->render(rc->deviceContext, Transform, 0.f, color);
}
//ç°ì˙ÇÕÇ±Ç±Ç‹Ç≈

void DropBox_Road::Gui()
{

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
    ImGui::SliderFloat("radius", &radius, 0.f, 1.f);
    ImGui::Text(ID.c_str());
}


bool DropBox_Road::isPlayerInRangeOf_Box()
{


    Camera& ince_c = Camera::instance();
    XMFLOAT2 camerapos = { ince_c.GetEye().x,ince_c.GetEye().z };
    XMFLOAT2 boxpos = { dropbox.oppnentPos.x,dropbox.oppnentPos.z };
    XMVECTOR Len = XMVectorSubtract(XMLoadFloat2(&boxpos), XMLoadFloat2(&camerapos));
    float len = XMVectorGetX(XMVector3LengthSq(Len));
    float Radius = radius;
    if (len < Radius * Radius)
    {
        return true;
    }


    return false;
}