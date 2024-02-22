#pragma once

#include"model.h"
#include"DirectXMath.h"
#include"Graphics/RenderContext.h"
using namespace DirectX;
#include"variable_management_class_for_hit_test.h"

enum class Obj_attribute
{
    cution,//軽い
    Super_cution,//超軽い
    heavy,//重い
    Super_heavy,//超重い

    Fragile,//壊れやすい
    Super_fragile,//超壊れやすい

    Hard_to_Break,//壊れにくい
    Super_hard_to_Break,//超壊れにくい
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
    virtual void Render(ID3D11DeviceContext* dc) = 0;
public:
    virtual const char* GetFbxName() { return nullptr; }
    Model* GetModel() { return this->model.get(); }

    XMFLOAT4X4 GetTransform() { return Transform; }
    XMFLOAT3 GetPosition() { return Position; }
    ObjType Get_attribute() { return old_attribute_state; };
    float GetReturnTimer() { return ReturnTimer; }
    Intersection GetIntersection() { return result_intersection; }
public:
    void SetPosition(XMFLOAT3 Pos) { Position = Pos; }
    void SetScale(XMFLOAT3 scale) { Position = scale; }
    void SetAngle(XMFLOAT3 angle) { Position = angle; }
    void SetTransform(XMFLOAT4X4 t) { Transform = t; }
    void Set_attribute(ObjType type) { old_attribute_state = type; };

protected:

    //性質の効果
    void ObjType_effect(float elapsedTime, XMFLOAT3& Pos);
    //自分が持っていたオリジナルの性質に戻す関数
    void Return_orijinal_ObjType(float elapsedTime);
    float ReturnTimer = 0.0f;
    ObjType original_attribute_state = ObjType::null;
    ObjType old_attribute_state = ObjType::null;
    Intersection result_intersection{};
    //この関数はコンストラクタ以外で使わない
    void initialaize_Set_attribute(ObjType type)
    {
        original_attribute_state = type;
        old_attribute_state = type;
    }
    void UpdateTransform();
    XMFLOAT3 Position{ 0,0,0 };
    XMFLOAT3 Velocty{ 0,0,0 };
    XMFLOAT3 Angle{ 0,0,0 };
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