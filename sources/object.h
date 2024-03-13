#pragma once

#include"model.h"
#include"DirectXMath.h"
#include"Graphics/RenderContext.h"
using namespace DirectX;
#include"variable_management_class_for_hit_test.h"
#include"Graphics/RenderContext.h"
#include"Collision.h"
#include"Mathf.h"
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
    ObjType Get_Objtype(int Number)const { return old_attribute_state[Number]; };
    float GetReturnTimer(int i)const { return ReturnTimer[i]; }
    Intersection GetIntersection()const { return result_intersection; }
    bool GetDestroyObje()const { return DestroyObj; }
    StageName Get_MyStageName()const { return stage_name; }//今自分がどのステージに置かれてるか取得
    XMFLOAT3 GetNormal()const { return Normal; }
    bool Get_isGimic_UpPosNow()const { return isGimic_UpPosNow; }
    XMFLOAT4 InitColor()const { return { 1.f,1.f,1.f,1.f }; }//orijinalcolor
public:
    void SetReturnTimer(float timer1 = 0.f, float timer2 = 0.f) { ReturnTimer[0] = timer2; ReturnTimer[1] = timer2; }
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
    void UpdateTransform();

    //垂直速力更新処理
    void UpdateVerticalVelocity(float elapsedFrame);

    //垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);

    //水平速力更新処理
    void UpdateHorizontalVelocity(float elapsedFrame);

    //水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);
protected:
    //移動処理
    void Move(float vx, float vz, float speed);

    //旋回処理
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //ジャンプ処理
    void Jump(float speed);

    //速力処理更新
    void UpdateVelocity(float elapsedTime);

    //着地したときに呼ばれる
    virtual void OnLanding() {}
protected:
    //重力
    float gravity = -1.0f;
    //地面に着地したか
    bool isGround = false;
    //摩擦
    float friction = 0.5f;

    //加速度
    float acceleration = 1.0f;

    //最大速度
    float maxMoveSpeed = 5.0f;

    //計算用
    float moveVecX = 0.0f;
    float moveVecZ = 0.0f;

    //空中移動
    float airControl = 0.3f;

    //開始位置を上にするため
    float stepOffset = 1.0f;

    //斜めの時間制限
    float slopeRate = 1.0f;
protected:
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