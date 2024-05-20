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
   
    void Child_UpdateTransform();
private:
    const char* filename = ".\\resources\\stage1\\stage_1.fbx";
    const char* filename2 = ".\\resources\\stage1\\tansu.fbx";


    DirectX::XMFLOAT3 child_Position{};
    DirectX::XMFLOAT3 child_Scale{1,1,1};
    DirectX::XMFLOAT3 child_Angle{};
    DirectX::XMFLOAT4X4 child_transform
    {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1    
    };
    unique_ptr<Model>child_model;
   
};


class Stage_1_2 :public Stage
{
public:
    Stage_1_2(ID3D11Device* device);
    ~Stage_1_2()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
private:
    const char* filename = ".\\resources\\stage3\\stage1_3.fbx";
};

class Stage_1_3 :public Stage
{
public:
    Stage_1_3(ID3D11Device*device);
    ~Stage_1_3();
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
private:
    const char* filename = ".\\resources\\stage5\\stage1_5.fbx";

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
    void WallSetID(std::string id_) { id = id_; }
    void SetIsWallFlagType(Gimic_Type Flagtype_) { Flagtype = Flagtype_; }
protected:
    Static_ObjType type = Static_ObjType::null;
    std::string id = "";
    Gimic_Type Flagtype = Gimic_Type::null;//この変数で見えない壁が消えるか消えないかを判断
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
