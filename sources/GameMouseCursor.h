#pragma once
#include"Graphics\sprite.h"
#include"memory"
#include<vector>
#include "scene_management.h"
class GameMauseCorsor
{
public:
    GameMauseCorsor() {};
    ~GameMauseCorsor() {};

    static GameMauseCorsor& Instance()
    {
        static GameMauseCorsor ins;
        return ins;
    };

    void Initialize();

    void Update();

    void Render(RenderContext* rc);
private:
    std::unique_ptr<sprite> mousespr;		//�}�E�X�̃X�v���C�g�`��
    const wchar_t* mousefilename = L".\\resources\\mouse\\���}�E�X.png";
    DirectX::XMFLOAT2 mousesprPos = { 0,0 };
    DirectX::XMFLOAT2 mousevel = { 0,0 };

    DirectX::XMFLOAT2 gamecursorPos = {};
    DirectX::XMFLOAT2 gamecurrentCursorPos = {};
};