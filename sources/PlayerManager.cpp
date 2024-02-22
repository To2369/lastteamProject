#include"PlayerManager.h"

void PlayerManager::Update(float elapsedTime)
{
    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        players[i]->Update(elapsedTime);
    }

#if false
    for (Player* player : remove)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�ō폜���Ȃ���΂Ȃ�Ȃ�
        auto it = std::find(players.begin(), players.end(),
            player);
        if (it != players.end())
        {
            players.erase(it);
        }
        delete player;
    }
#endif
    // �j�����X�g���N���A
    remove.clear();
}

void PlayerManager::Render(ID3D11DeviceContext* dc)
{
    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        players[i]->Render(dc);
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