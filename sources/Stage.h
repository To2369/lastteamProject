#pragma once
#include"object.h"
class Stage :public Object
{
public:
    Stage() {};
    virtual ~Stage() {};

    virtual void Update(float elapsedTime) = 0;
    virtual void Render(RenderContext* rc) = 0;
   
public:
    void Set_Color(XMFLOAT4 color_) { color = color_; }
public:
    ObjType Get_GameObjType() { return get_gameobj; }
    XMFLOAT4 GetColor() { return color; }
private:
    ObjType get_gameobj = ObjType::null;
    StageName s_name = StageName::null;
};

class Stage_1_1 :public Stage
{
public:
    Stage_1_1(ID3D11Device* device);
    ~Stage_1_1()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
private:
    const char* filename = ".\\resources\\stage1\\stage_1.fbx";
};


class Stage_1_3 :public Stage
{
public:
    Stage_1_3(ID3D11Device* device);
    ~Stage_1_3()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
private:
    const char* filename = ".\\resources\\stage3\\stage1_3.fbx";
};





class stage_Yuka :public Stage
{
public:
    stage_Yuka(ID3D11Device* device);
    ~stage_Yuka()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";

};



enum class Static_Object_name
{
    Static_Object,
    Invisible_Wall,
    null
};
using Static_ObjType = Static_Object_name;
//静的オブジェクトのクラス
class Static_Object :public Object
{
public:
    Static_Object() {};
    virtual ~Static_Object()override {};
    void Update(float elapsedtime)override {};
    void Render(RenderContext* rc)override {};
    void Gui()override {};
    Static_ObjType GetStatic_ObjType() { return type; }

protected:
    Static_ObjType type = Static_ObjType::null;
};



class stage_OBJ :public Static_Object
{
public:
    stage_OBJ(ID3D11Device* device);
    stage_OBJ(ID3D11Device* device, const char* filename_);
    ~stage_OBJ()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
private:
    DirectX::XMFLOAT4X4 boxTransform{
         1,0,0,0
        ,0,1,0,0
        ,0,0,1,0,
         0,0,0,1 };
    unique_ptr<Model>model_box;
    const char* filename = ".\\resources\\stage1\\tana.fbx";
};
