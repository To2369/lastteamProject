#include"PlayerManager.h"
#include"Graphics/DebugRenderer.h"
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
    // �j�����X�g���N���A
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
        //�ʒu
        DirectX::XMFLOAT3 plPos = players[i]->GetPosition();
        ImGui::InputFloat3("position", &plPos.x);
        //��]
        DirectX::XMFLOAT3 plAngle = *players[i]->GetAngle();
        DirectX::XMFLOAT3 a{ DirectX::XMConvertToDegrees(plAngle.x), DirectX::XMConvertToDegrees(plAngle.y) , DirectX::XMConvertToDegrees(plAngle.z) };
        ImGui::InputFloat3("Angle", &a.x);
        plAngle = { DirectX::XMConvertToRadians(a.x),DirectX::XMConvertToRadians(a.y), DirectX::XMConvertToRadians(a.z) };

        //�g��k��
        DirectX::XMFLOAT3 plScale = *players[i]->GetScale();
        ImGui::InputFloat3("Scale", &plScale.x);

        DirectX::XMFLOAT3 plvel = *players[i]->getVelocity();
        ImGui::InputFloat3("Vel", &plvel.x);

        DirectX::XMFLOAT3 pla = *players[i]->geta();
        ImGui::InputFloat3("a", &pla.x);
    }
    ImGui::End();
}

void PlayerManager::drawDrawPrimitive(ID3D11Device*device)
{
    DebugRenderer debugRenderer = DebugRenderer::incetance(device);

    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        //�Փ˔���p�̃f�o�b�O����`��
        debugRenderer.DrawSphere(players[i]->GetPosition(), players[i]->getRadius(), { 0,0,0,1 });
    }
}