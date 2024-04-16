#pragma once

#include"model.h"
#include"DirectXMath.h"
#include"Graphics/RenderContext.h"


#include"variable_management_class_for_hit_test.h"
#include"Graphics/RenderContext.h"
#include"Mathf.h"
enum class UI_attribute
{
    Goal_Arrow,

};
enum class Obj_attribute
{
    cution,//軽い//作った
    Super_cution,//超軽い
    heavy,//重い//作った
    Super_heavy,//超重い

    Fragile,//壊れやすい
    Super_fragile,//超壊れやすい

    Hard_to_Break,//壊れにくい
    Super_hard_to_Break,//超壊れにくい

    Crack,//ひび割れ

    null,
};
enum class StageName
{
    Tutolial,
    stage1_1,
    null,
};
enum class Gimic_Type
{
    Switch,
    Door,
    Goal,
    Drop_Road,
    null
};
//Guiをマジックナンバーを使用して操作しないよう
struct Gui_parameter_Valu
{
   const DirectX::XMFLOAT3 Max{0.5f,0.5f,0.5f};
   const DirectX::XMFLOAT3 Min{-0.5f,-0.5f,-0.5f};
};
using ObjType = Obj_attribute;
using UIType = UI_attribute;
//ゲームで使う置物の基底クラス
class Object
{
public:
    Object()
    {

    };
    virtual ~Object() {};
    virtual void Update(float elapsedTime) = 0;
    virtual void Render(RenderContext* rc) = 0;
public:
    virtual void Gui() {};
public:
    Model* GetModel()const { return this->model.get(); }

    DirectX::XMFLOAT4X4 GetTransform()const { return Transform; }
    DirectX::XMFLOAT3 GetPosition()const { return Position; }
    DirectX::XMFLOAT3 GetScale() const { return Scale; }
    ObjType Get_Old_Objtype(int Number)const { return old_attribute_state[Number]; };//この関数がリアルタイムで更新されるType
    ObjType Get_Original_Objtype(int Number)const { return original_attribute_state[Number]; };//定数Type
    float GetReturnTimer(int i)const { return ReturnTimer[i]; }
    Intersection GetIntersection()const { return result_intersection; }
    bool GetDestroyObje()const { return DestroyObj; }
    StageName Get_MyStageName()const { return stage_name; }//今自分がどのステージに置かれてるか取得
    DirectX::XMFLOAT3 GetNormal()const { return Normal; }
    bool Get_isGimic_UpPosNow()const { return isGimic_UpPosNow; }
    //今自分が起動してるIDを取得
   std::string Get_BootGimicType()const { return GetBootGimicType_ID; }
    DirectX::XMFLOAT4 InitColor()const { return { 1.f,1.f,1.f,1.f }; }//orijinalcolor
public:
    void SetVeloty(DirectX::XMFLOAT3 sp) { Velocty = sp; }
    void SetReturnTimer(float timer1 = 0.f, int num=0) { ReturnTimer[num] =timer1; }
    void SetColor(DirectX::XMFLOAT4 color_) { color = color_; }
    void SetPosition(XMFLOAT3 Pos) { Position = Pos; }
    void SetScale(DirectX::XMFLOAT3 scale) { Scale = scale; }
    void SetAngle(DirectX::XMFLOAT3 angle) { Angle = angle; }
    void SetTransform(DirectX::XMFLOAT4X4 t) { Transform = t; }
    void Set_attribute(ObjType type,int Number) { 
        old_attribute_state[Number]=type;
    };
    void Set_MystageName(StageName name) { stage_name = name; }
    void Destroy() { DestroyObj = true; }//オブジェクト破棄するための関数
    void SetNormal(DirectX::XMFLOAT3 normal) { Normal = normal; };
    void Set_isGimic_UpPosNow(bool f) { isGimic_UpPosNow = f; }
    void Set_GimicType(string g) { GetBootGimicType_ID = g; }
protected:
    
   virtual void UpdateTransform();
    void box_Collition_obj();
    void RayCastGround();
    //性質の効果
    void ObjType_effect(float elapsedTime);
    //自分が持っていたオリジナルの性質に戻す関数
    void Return_orijinal_ObjType(float elapsedTime);
    //この関数はコンストラクタ以外で使わない
    void initialaize_Set_attribute(ObjType type1, ObjType type2)
    {
        original_attribute_state.push_back(type1);
        original_attribute_state.push_back(type2);
        old_attribute_state.push_back(type1);
        old_attribute_state.push_back(type2);
    }
    DirectX::XMFLOAT3 Normal{ 0.f,0.f,0.f };//raycast用
    DirectX::XMFLOAT3 Position{ 0.f,0.f,0.f };
    DirectX::XMFLOAT3 Velocty{ 0.f,0.f,0.f };
    DirectX::XMFLOAT3 Angle{ 0.f,0.f,0.f };
    DirectX::XMFLOAT3 Scale{ 1.f,1.f,1.f };
    DirectX::XMFLOAT4 color{ 1.f,1.f,1.f,1.f };
    DirectX::XMFLOAT4X4 Transform
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };
    std::unique_ptr<Model> model;
private:
    std::string GetBootGimicType_ID = "null";
    bool isGimic_UpPosNow = false; //いま自分がギミックの上の方のバウンディングボックスにあったってるかどうか,ギミックに対してでしか使わない
    bool DestroyObj = false;
    bool NotUpdateFlag = false;//gimic_VS_Object関数にしか使わない予定
    float ReturnTimer[2]{};
    StageName stage_name = StageName::null;
    //todo:ここをvectorにする必要がないので手が空き次第配列に書き換える
   vector<ObjType> original_attribute_state;
   vector<ObjType> old_attribute_state;
    Intersection result_intersection{};
protected://gui
 
    Gui_parameter_Valu parameter_valu{};
};