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

public:
    bool hitChechLect(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 pos2, DirectX::XMFLOAT2 size1, DirectX::XMFLOAT2 size2);
private:
    std::unique_ptr<sprite> padcursorspr;		//マウスのスプライト描画
    const wchar_t* Padcoursorfilename = L".\\resources\\mouse\\仮マウス.png";
    DirectX::XMFLOAT2 PadCursorsprPos = { 0,0 };

    DirectX::XMFLOAT2 PadcursorPos = {};
    DirectX::XMFLOAT2 currentPadCursorPos = {};

    float speed = 0;

    struct square
    {
        float left, right, top, bottom;
    };
};