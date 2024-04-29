#include"PlayerManager.h"
#include"Graphics/DebugRenderer.h"
#include"object.h"
#include<string>
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
void PlayerManager::Update(float elapsedTime)
{
    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        players[i]->update(elapsedTime);
    }
    // 破棄リストをクリア
    remove.clear();
}

void PlayerManager::Render(RenderContext* rc)
{
    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        players[i]->render(rc);
    }
}

void PlayerManager::Clear()
{
    players.clear();
}

void PlayerManager::Remove(Player* player)
{
    remove.insert(player);
}

void PlayerManager::DrawDebugGui()
{
    ImGui::Begin("Player");

    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        using namespace DirectX;
        //位置
        DirectX::XMFLOAT3 plPos = players[i]->GetPosition();
        ImGui::InputFloat3("position", &plPos.x);
        //回転
        DirectX::XMFLOAT3 plAngle = players[i]->GetAngle();
        DirectX::XMFLOAT3 a{ DirectX::XMConvertToDegrees(plAngle.x), DirectX::XMConvertToDegrees(plAngle.y) , DirectX::XMConvertToDegrees(plAngle.z) };
        ImGui::InputFloat3("Angle", &a.x);
        plAngle = { DirectX::XMConvertToRadians(a.x),DirectX::XMConvertToRadians(a.y), DirectX::XMConvertToRadians(a.z) };

        //拡大縮小
        DirectX::XMFLOAT3 plScale = players[i]->GetScale();
        ImGui::InputFloat3("Scale", &plScale.x);

        //velocity
        DirectX::XMFLOAT3 plVelocity = players[i]->GetVelocity();
        ImGui::InputFloat3("Velocity", &plVelocity.x);

        std::string str = "";
        ObjType platt = players[i]->Getattribute();
        switch (platt)
        {
        case Obj_attribute::cution:
            str = "cution";
            break;
        case Obj_attribute::Super_cution:
            str = "super cution";
            break;
        case Obj_attribute::heavy:
            str = "hevy";
            break;
        case Obj_attribute::Super_heavy:
            str = "super hevy";
            break;
        case Obj_attribute::Fragile:
            str = "Fragile";
            break;
        case Obj_attribute::Super_fragile:
            str = "Super_fragile";
            break;
        case Obj_attribute::Hard_to_Break:
            str = "hard_to_break";
            break;
        case Obj_attribute::Super_hard_to_Break:
            str = "super_hard_to_Break";
            break;
        case Obj_attribute::Crack:
            str = "crack";
            break;
        case Obj_attribute::null:
            str = "null";
            break;
        }
        ImGui::Text(u8"platt　%s", str.c_str());
        /*DirectX::XMFLOAT3 plvel = *players[i]->GetVelocity();
        ImGui::InputFloat3("Vel", &plvel.x);*/
    }
    ImGui::End();
}

void PlayerManager::drawDrawPrimitive(ID3D11Device*device)
{
    DebugRenderer debugRenderer = DebugRenderer::incetance(device);

    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        //衝突判定用のデバッグ球を描画
        debugRenderer.DrawSphere(players[i]->GetPosition(), players[i]->getRadius(), { 0,1,0,1 });
    }
}