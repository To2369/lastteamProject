#pragma once
#include"UI.h"
#include"object.h"
namespace failepath
{
    
   static wstring UI_Path_Wstring = L".\\resources\\UI\\";//UIまでのパス
   static wstring UI_Bottun_Other_Path_Wstring = L".\\resources\\UI\\Bottun_Other\\";//Bottun_Otherまでのパス
   static wstring UI_Mask_Path_Wstring = L".\\resources\\UI\\Mask\\";//Bottun_Otherまでのパス
   static wstring UI_Player_Property = L".\\resources\\UI\\player_property\\"; //player_propertyまでのパス
   namespace Stage_failepath
   {
       static string  Stage1_Path = ".\\resources\\Stage1\\";//stage1までのパス
       static string  Stage3_Path = ".\\resources\\Stage3\\";//stage3までのパス
       static string  Stage5_Path = ".\\resources\\Stage5\\";//stage5までのパス
   };
   namespace Object_failepath
   {
       static string  Pengin = ".\\resources\\stage3\\object\\pengin.fbx";//ペンギンのオブジェクト
       static string  Kami =   ".\\resources\\stage3\\object\\kami.fbx";//紙のオブジェクト
       static string  Denti =  ".\\resources\\stage3\\object\\denti.fbx";//電池のオブジェクト
       
       struct ObjectName
       {
           string name="";
           string filename = "";
       };
       namespace Gimic
       {
           static ObjectName  thissyu = { "thissyu" ,Stage_failepath::Stage5_Path + "objects\\thissyu.fbx" };//ティッシュのオブジェクト
           static ObjectName  hikidasi_k = { "hikidasi_k",Stage_failepath::Stage5_Path + "objects\\hikidasi_K.fbx" };//引き出しのオブジェクト

       };
       namespace Object
       {
           static ObjectName  Book = { "Book",Stage_failepath::Stage5_Path + "objects\\book.fbx" };//本のオブジェクト
           static ObjectName  E_kezuri = { "E_kezuri",Stage_failepath::Stage5_Path + "objects\\E_kezuri.fbx" };//鉛筆削りのオブジェクト
           static ObjectName  kesigomu = { "kesigomu",Stage_failepath::Stage5_Path + "objects\\kesigomu.fbx" };//消しゴムのオブジェクト
           static ObjectName  kyappu = { "kyappu",Stage_failepath::Stage5_Path + "objects\\kyappu.fbx" };//キャップのオブジェクト
           static ObjectName  enpitu = { "enpitu",Stage_failepath::Stage5_Path + "objects\\enpitu.fbx" };//鉛筆のオブジェクト
           static ObjectName  Light = { "right",Stage_failepath::Stage5_Path + "objects\\right.fbx" };//ライトのオブジェクト

       };

      
   };
   
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
    void Clear(std::string id) {
        for (auto& canbas : CanBass)
        {
            if (id == canbas->GetCanbasID())
            {
                canbas->UIClear();
                break;
            }
        }
    }
    void CreateCanbas();
    void CreateCanbas(std::string id);
    void CreateGameSceneUI(ID3D11Device*device);
    void CreateUI(ID3D11Device*device,ObjType type, vector<unique_ptr<UI>>& uis);
    void CreateUI(ID3D11Device*device,std::wstring filename,
        ObjType type,std::string id ,vector<unique_ptr<UI>>& uis,DirectX::XMFLOAT2 pos);
    
    void Gui();
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    void Render(RenderContext* rc,std::string id);
    void Render(RenderContext* rc, std::string canbas_id, std::string ui_id);
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
