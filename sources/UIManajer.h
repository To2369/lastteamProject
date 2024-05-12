#pragma once
#include"UI.h"
#include"object.h"
namespace filepath
{
    
   static wstring UI_Path_Wstring = L".\\resources\\UI\\";//UIまでのパス
   static wstring UI_Bottun_Other_Path_Wstring = L".\\resources\\UI\\Bottun_Other\\";//Bottun_Otherまでのパス
   static string  Stage1_Path = ".\\resouces\\Stage1\\";//stage1までのパス
   static wstring  Wstring_png = L".png";//.png
   static string  string_png = ".png";//.png
}
class UIManager
{
public:
    ~UIManager()
    {
       
    }
    static UIManager& incetance()
    {
        static UIManager ince;
        return ince;
    };
    void Clear() {
       CanBass.clear();
    }
    void CreateCanbas();
    void CreateCanbas(std::string id);
    void CreateGameSceneUI(ID3D11Device*device);
    
    void Gui();
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    void Render(RenderContext* rc,std::string id);
    //uiのvectoをこのマネージャーにある、UIsに移動させる
    void UI_move(vector<unique_ptr<UI>>ui) { UIs=move(ui); }
    /// <summary>
   /// 
   /// </summary>
   /// <param name="SP_POS">spritePos</param>
   /// <param name="SP_Scale">spriteScale</param>
   /// <returns></returns>
    bool Mouse_VS_UI(DirectX::XMFLOAT2 SP_POS, D3D11_TEXTURE2D_DESC& SP_desc);
    bool Mouse_VS_UI(DirectX::XMFLOAT2 SP_POS, XMFLOAT2 Scale);
public:
    ObjType Get_Objetypes(int i) { return ObjectTypes[i]; }
    int GetCanBassCount() { return CanBass.size(); }
    CanBas* GetCanbas(int i) { return CanBass[i].get(); }
private:
    void GetStageObjectTypes();
    std::vector<ObjType>ObjectTypes;
    std::vector<unique_ptr<CanBas>>CanBass;
private:
    std::vector<unique_ptr<UI>>UIs;
};
