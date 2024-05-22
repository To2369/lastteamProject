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

    void Update(float elapsedTime);

    void Render(RenderContext* rc);

public:
    bool hitChechLect(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 pos2, DirectX::XMFLOAT2 size1, DirectX::XMFLOAT2 size2);

    DirectX::XMFLOAT2 GetPadCursorsprPos(){return PadCursorsprPos;}
    void SetPadCursorsprPos(const DirectX::XMFLOAT2 pos) { PadCursorsprPos = pos; }
private:
    std::unique_ptr<sprite> padcursorspr;		//マウスのスプライト描画
    const wchar_t* Padcoursorfilename = L".\\resources\\mouse\\mouse.png";
    DirectX::XMFLOAT2 PadCursorsprPos = { 0,0 };

    DirectX::XMFLOAT2 PadcursorPos = {};
    DirectX::XMFLOAT2 currentPadCursorPos = {};
    DirectX::XMFLOAT2 corsor = {};

    float speed = 0;

    struct square
    {
        float left, right, top, bottom;
    };
    DirectX::XMFLOAT4 color = { 0,0,0,0 };

    bool padCorsorFlag = false;
    bool disPlayFlag = false;

    float timer = 0;
    float timerset = 20;
};