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
        //std::vectorから要素を削除する場合はイテレータで削除しなければならない
        auto it = std::find(players.begin(), players.end(),
            player);
        if (it != players.end())
        {
            players.erase(it);
        }
        delete player;
    }
#endif
    // 破棄リストをクリア
    remove.clear();
}

void PlayerManager::Render(RenderContext* rc)
{
    int count = players.size();
    for (int i = 0; i < count; i++)
    {
        players[i]->Render(rc);
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