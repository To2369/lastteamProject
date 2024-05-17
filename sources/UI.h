#pragma once
#include "Graphics/RenderContext.h"
#include"Graphics/sprite.h"
#include <vector>
#include"memory"

namespace UI_StringID
{
    namespace UI_ID
    {
        //scene title ui id
        namespace Title_ID
        {
            static std::string Start = "Start";
            static std::string End = "End";
            static std::string Back = "Back";
        };
        //scene stage select ui id
        namespace StageSelect_ID
        {
            static std::string Stage1 = "Stage1";
            static std::string Stage2 = "Stage2";
            static std::string Stage3 = "Stage3";
        };
        //scene game menu ui id
        namespace Menu_Id
        {
            static std::string MenuRitrai = "MenuRitrai";
            static std::string MenuContinue = "MenuContinue";
            static std::string MenuGibuUp = "MenuGibuUp";
        };
        //scene clear ui id
        namespace Clear_Id
        {
            static std::string ClearClearLogo = "Clear_ClearLogo";
            static std::string ClearTitle = "ClearTitle";
            static std::string ClearStageSelect = "ClearStageSelectButton";
            static std::string ClearNextStage = "ClearNextStage";
        };
        namespace Mask_Id
        {
            static std::string Black = "Black";
        };
    };
    //scene game canbasID
    namespace CanbasID
    {
        static std::string SceneGameUI = "SceneGameUI";
        static std::string Menu = "Menu";
        static std::string GameClear = "GameClear";
        static std::string Mask = "Mask";
        static std::string Player = "Player";
    };

};
class UI;
class CanBas//こいつを中心にしてUI配置
{
public:
    CanBas(std::vector<std::unique_ptr<UI>>&uis);
    CanBas(std::vector<std::unique_ptr<UI>>&uis,std::string canbasID_);
    ~CanBas();
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    void Render(RenderContext* rc,std::string ui_id);
    void SetOrijinPos(DirectX::XMFLOAT2 pos) { MainPos = pos; };
    void SetCanbasID(std::string id) { canbasID = id; }
    const DirectX::XMFLOAT2 GetOrijinPos()const{ return MainPos; };
    void Gui();
    void ResultInfo();
public:
    int GetUICount() { return Uis.size(); }
    std::string GetCanbasID() { return canbasID; }
    UI* GetUI(int i) { return Uis[i].get(); }
    void UIClear();
private:
    std::string canbasID = "";
    DirectX::XMFLOAT2 MainPos{};
    std::vector<std::unique_ptr<UI>>Uis;
    int random_value = 0;
};

class UI
{
public:
    UI(ID3D11Device* device, const wchar_t* filename, DirectX::XMFLOAT2 scale = {}, DirectX::XMFLOAT2 pos = {});
    ~UI() {};
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    void Gui();
public:
    void SetPosition(DirectX::XMFLOAT2 pos) { Pos = pos; };
    void SetScale(DirectX::XMFLOAT2 scale) { Scale = scale; }
    void SetCanBasPosition(DirectX::XMFLOAT2 canbasP) { canbasPos = canbasP; };
    void SetHanteiFlag(bool f) { hanteiflag = f; }
    void SetIsMouse(bool f) { Ismouse = f; }
    void SetID(std::string id) { ID = id; }
    void SetColor(DirectX::XMFLOAT4 color) { Color = color; };
public:
    DirectX::XMFLOAT2 GetPosition() { return sp_pos; }
    std::string GetID() { return ID; }
    bool GetHanteiFlag(){ return hanteiflag; }
    DirectX::XMFLOAT2 GetScale() { return Scale; };
    D3D11_TEXTURE2D_DESC& GetTexture2DDesc() { return Ui.get()->texture2d_desc; }
public:
   
private:
    std::string ID;
    //当たり判定で使う正確なスプライトの位置
    DirectX::XMFLOAT2 sp_pos;
    DirectX::XMFLOAT4 Color{1,1,1,1};
    DirectX::XMFLOAT2 canbasPos;
    DirectX::XMFLOAT2 Pos;
    DirectX::XMFLOAT2 Scale;
    std::unique_ptr<sprite>Ui;
    std::string filename_gui;
    int random_value;
    bool hanteiflag=false;
    bool Ismouse = false;
    bool hanten = false;
    float Color_alpha = 0.01f;
};