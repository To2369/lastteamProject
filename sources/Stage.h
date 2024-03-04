#pragma once
#include"object.h"

class Stage:public Object
{
public:
    Stage() {};
    virtual ~Stage(){};
 
    virtual void Update(float elapsedTime)=0;
    virtual void Render(RenderContext* rc)=0;
    //ステージに対してオブジェクトのレイキャスト処理
   // bool modelRay_VS_Stage(Intersection& inter, XMFLOAT3 FastPos, XMFLOAT3 normal/*レイの向き*/, float rayLength/*レイの長さ*/);
    void SetStage(StageName n) { s_name = n; }
    StageName GetNowStage() { return s_name; }
public:
    void Set_Color(XMFLOAT4 color_) { color = color_; }
public:
    ObjType Get_GameObjType() { return get_gameobj; }
    XMFLOAT4 GetColor() { return color; }
private:
    ObjType get_gameobj = ObjType::null;
    StageName s_name = StageName::stage1_1;
};

class Stage_1_1 :public Stage
{
public:
    Stage_1_1(ID3D11Device*device);
    ~Stage_1_1()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
private:
    const char* filename = ".\\resources\\ground.fbx";

};