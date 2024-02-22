#pragma once
#pragma once
#include"object.h"
enum class StageName
{
    Tutolial,
    stage1,
    null,
};
class DebugStage :public Object
{
public:
    DebugStage(ID3D11Device*device);
    ~DebugStage()override {};
    void Update(float elapsedTime)override;
    void Render(ID3D11DeviceContext* dc)override;
    //ステージに対してオブジェクトのレイキャスト処理
    bool modelRay_VS_Stage(Intersection& inter, XMFLOAT3 FastPos, XMFLOAT3 normal/*レイの向き*/, float rayLength/*レイの長さ*/);
    void SetStage(StageName n) { s_name = n; }
    StageName GetNowStage() { return s_name; }
public:
    //今スイッチに乗ってるオブジェのをセット
    void Set_GameObjType(ObjType obj) { get_gameobj = obj; };
    void Set_Color(XMFLOAT4 color_) { color = color_; }
public:
    ObjType Get_GameObjType() { return get_gameobj; }
    XMFLOAT4 GetColor() { return color; }
private:
    ObjType get_gameobj = ObjType::null;
    //const char* filename = ".\\resources\\3Dmodel\\room.fbx";
    const char* filename = ".\\resources\\ground.fbx";
    StageName s_name = StageName::stage1;
};
