#pragma once
#include"Graphics\sprite.h"
#include"memory"
#include<vector>
#include "scene_management.h"
class GamePadCorsor
{
public:
    GamePadCorsor() {};
    ~GamePadCorsor() {};

    static GamePadCorsor& Instance()
    {
        static GamePadCorsor ins;
        return ins;
    };

    void Initialize();

    void Update();

    void Render(RenderContext* rc);
private:
    std::unique_ptr<sprite> padcursorspr;		//�}�E�X�̃X�v���C�g�`��
    const wchar_t* Padcoursorfilename = L".\\resources\\mouse\\���}�E�X.png";
    DirectX::XMFLOAT2 PadCursorsprPos = { 0,0 };
    DirectX::XMFLOAT2 PadCoursorvelocity = { 0,0 };

    DirectX::XMFLOAT2 PadcursorPos = {};
    DirectX::XMFLOAT2 currentPadCursorPos = {};

    float speed = 0;

    bool resetFlag = false;
    bool LimitFlag = false;

};