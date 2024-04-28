#pragma once
#include "Graphics/RenderContext.h"
#include"Graphics/sprite.h"
#include <vector>
#include"memory"

class UI;
class CanBas//Ç±Ç¢Ç¬ÇíÜêSÇ…ÇµÇƒUIîzíu
{
public:
    CanBas(std::vector<std::unique_ptr<UI>>&uis);
    ~CanBas();
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    void SetOrijinPos(DirectX::XMFLOAT2 pos) { MainPos = pos; };
    const DirectX::XMFLOAT2 GetOrijinPos()const{ return MainPos; };
    void Gui();
private:
    DirectX::XMFLOAT2 MainPos{};
    std::vector<std::unique_ptr<UI>>Uis;

};

class UI
{
public:
    UI(ID3D11Device* device, const wchar_t* filename, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 pos);
    ~UI() {};
    void Render(RenderContext* rc);
    void SetPosition(DirectX::XMFLOAT2 pos) { Pos = pos; };
    void SetCanBasPosition(DirectX::XMFLOAT2 canbasP) { canbasPos = canbasP; };
    void Gui();

private:
    DirectX::XMFLOAT2 canbasPos;
    DirectX::XMFLOAT2 Pos;
    DirectX::XMFLOAT2 Scale;
    std::unique_ptr<sprite>Ui;
    std::string filename_gui;
    int random_value;
};