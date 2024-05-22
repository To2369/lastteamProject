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
    //カーソルと四角形の当たり判定
    bool hitChechLect(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 pos2, DirectX::XMFLOAT2 size1, DirectX::XMFLOAT2 size2);

    DirectX::XMFLOAT2 GetPadCursorsprPos(){return PadCursorsprPos;}
    void SetPadCursorsprPos(const DirectX::XMFLOAT2 pos) { PadCursorsprPos = pos; }
private:
    std::unique_ptr<sprite> padcursorspr;		//マウスのスプライト描画
    const wchar_t* filename = L".\\resources\\mouse\\padcursor.png";      //カーソルのファイル
    DirectX::XMFLOAT2 PadCursorsprPos = { 0,0 };        //パッドカーソルの現在位置

    DirectX::XMFLOAT2 PadcursorPos = {};                //マウスカーソルの現在位置
    DirectX::XMFLOAT2 currentPadCursorPos = {};

    float speed = 0;

    struct square
    {
        float left, right, top, bottom;
    };
    DirectX::XMFLOAT4 color = { 0,0,0,0 };

    bool padCorsorFlag = false;     //パッドカーソルを使うか
    bool disPlayFlag = false;       //パッドカーソルを表示するか

    float timer = 0;                //パッドカーソルが消えるまでの時間をセットする場所
    float timerset = 20;            //パッドカーソルが消えるまでの時間
};