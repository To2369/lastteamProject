#pragma once

#include"model.h"
#include"DirectXMath.h"
#include"Graphics/RenderContext.h"
using namespace DirectX;
#include"variable_management_class_for_hit_test.h"
#include"Graphics/RenderContext.h"

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

    null,
};
enum class StageName
{
    Tutolial,
    stage1_1,
    null,
};


using ObjType = Obj_attribute;
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
    Model* GetModel()const { return this->model.get(); }

    XMFLOAT4X4 GetTransform()const { return Transform; }
    XMFLOAT3 GetPosition()const { return Position; }
    XMFLOAT3 GetScale() const { return Scale; }
    ObjType Get_attribute(int Number)const { return old_attribute_state[Number]; };
    float GetReturnTimer(int i)const { return ReturnTimer[i]; }
    Intersection GetIntersection()const { return result_intersection; }
    bool GetDestroyObje()const { return DestroyObj; }
    StageName Get_MyStageName()const { return stage_name; }//今自分がどのステージに置かれてるか取得
    XMFLOAT3 GetNormal()const { return Normal; }
    bool Get_isGimic_UpPosNow()const { return isGimic_UpPosNow; }
    XMFLOAT4 InitColor()const { return { 1.f,1.f,1.f,1.f }; }//orijinalcolor
public:
    void SetColor(XMFLOAT4 color_) { color = color_; }
    void SetPosition(XMFLOAT3 Pos) { Position = Pos; }
    void SetScale(XMFLOAT3 scale) { Scale = scale; }
    void SetAngle(XMFLOAT3 angle) { Angle = angle; }
    void SetTransform(XMFLOAT4X4 t) { Transform = t; }
    void Set_attribute(ObjType type,int Number) { old_attribute_state[Number]=type; };
    void Set_MystageName(StageName name) { stage_name = name; }
    void Destroy() { DestroyObj = true; }
    void SetNormal(XMFLOAT3 normal) { Normal = normal; };
    void Set_isGimic_UpPosNow(bool f) { isGimic_UpPosNow = f; }
protected:
    void box_Collition_obj();
    void RayCastGround();
    //性質の効果
    void ObjType_effect(float elapsedTime);
    //自分が持っていたオリジナルの性質に戻す関数
    void Return_orijinal_ObjType(float elapsedTime);
    void UpdateTransform();
    //この関数はコンストラクタ以外で使わない
    void initialaize_Set_attribute(ObjType type1, ObjType type2)
    {
        original_attribute_state.push_back(type1);
        original_attribute_state.push_back(type2);
        old_attribute_state.push_back(type1);
        old_attribute_state.push_back(type2);
    }
    bool isGimic_UpPosNow = false; //いま自分がギミックの上の方のバウンディングボックスにあったってるかどうか,とギミックに対してでしか使わない
    bool DestroyObj = false;
    float ReturnTimer[2]{};
    StageName stage_name = StageName::null;
   vector<ObjType> original_attribute_state;
   vector<ObjType> old_attribute_state;
    Intersection result_intersection{};

    XMFLOAT3 Normal{ 0.f,0.f,0.f };//raycast用
    XMFLOAT3 Position{ 0.f,0.f,0.f };
    XMFLOAT3 Velocty{ 0.f,0.f,0.f };
    XMFLOAT3 Angle{ 0.f,0.f,0.f };
    XMFLOAT3 Scale{ 1.f,1.f,1.f };
    XMFLOAT4 color{ 1.f,1.f,1.f,1.f };
    XMFLOAT4X4 Transform
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };
    std::unique_ptr<Model> model;
  
};