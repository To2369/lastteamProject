#pragma once
#include"Player.h"
#include<set>
#include<vector>
#include<DirectXMath.h>

class PlayerManager
{
public:
    PlayerManager(){}
    ~PlayerManager(){}

    static PlayerManager& Instance()
    {
        static PlayerManager ins;
        return ins;
    }

    void Update(float elpasedTime);
    void Render(RenderContext* rc);

    void Clear();

    void Remove(Player* player);
    void Register(std::unique_ptr<Player> player) { players.emplace_back(std::move(player)); }
    int GetPlayerCount() { return players.size(); }
    Player* GetPlayer(int num) { return players.at(num).get(); }
private:
    vector<unique_ptr<Player>> players;
    set<Player*>remove;
};