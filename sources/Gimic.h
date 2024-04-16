#pragma once
#include"object.h"
#include"Graphics/RenderContext.h"

class Gimic :public Object
{
public:
    Gimic() {};
    virtual ~Gimic() {};

    virtual void Update(float elapsedTime) =0;
    virtual void Render(RenderContext* rc) =0;
    
    //自分のタイプを取得
    Gimic_Type Get_GimicType()const { return Gimic_type; }
public:
    virtual bool gimic_VS_Object() { return false; };//今のところswitchクラスに対してしか使っていない
    void SetSwitchFlag(bool f) { switchFlag = f; }//今のところswitchクラスに対してしか使っていない
    bool Get_SwitchFlag()const { return switchFlag; }//今のところswitchクラスに対してしか使っていない
public:
    ObjType GetMyObjeFlagType()const { return MyObjeFlagType; }
    Gimic_Type GetMyGimicFlagType() { return MyGimicFlagType; }
    void SetGimicID(string id) { ID = id; }//例：複数のスイッチが出てきた時にこれがないと、一つのスイッチでスイッチで起動するすべてのギミックが起動してしまう
    string GetGimicID() { return ID;}//この関数を使って自分(ギミック)がどのギミックに対して起動するかを識別する
public:
  
 
    virtual collision_mesh* GetHitBoxMesh() { return nullptr; }//今のところDropBoxクラスに対してしか使っていない
    virtual bool GetPlayerStopFlag(){ return false; };//今のところDropBoxクラスに対してしか使っていない
    virtual XMFLOAT4X4 GetBoxTransForm() { return XMFLOAT4X4(); }//今のところDropBoxクラスに対してしか使っていない
public:
    virtual void Gimic_VS_GimicFlagBoot() {};//今のところDoorクラスに対してしか使っていない
    bool GetBootFlag()const { return bootFlag[0]; }//今のところDoorクラスに対してしか使っていない
    bool GetBootFlag(int i)const { return bootFlag[i]; }//今のところDoorクラスに対してしか使っていない
    void SetBootFlag(bool f) { bootFlag.push_back(f); }//今のところDoorクラスに対してしか使っていない
public:
  

protected:
    string ID;
    vector<bool> bootFlag;
    bool switchFlag=false;
    void Gimic_effect(Gimic_Type type);
    //自分のタイプ
    Gimic_Type Gimic_type=Gimic_Type::null;
    //ObjType get_gameobj;
    ObjType MyObjeFlagType=ObjType::null;//自分がどのオブジェクトタイプでギミックが起動するかの変数
    Gimic_Type MyGimicFlagType = Gimic_Type::null;//自分がどのGimicタイプで自分のギミックが起動するかの変数

};

class Switch :public Gimic
{
public:
    Switch(ID3D11Device* device);
    ~Switch()override {
      
    };
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    bool gimic_VS_Object()override;
    void Gui()override;
  

private:
    const char* filename = ".\\resources\\stage1\\bottan.fbx";
};


class Door :public Gimic
{
public:
    Door(ID3D11Device* device);
    ~Door()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gimic_VS_GimicFlagBoot()override;
    void Gui()override;


public:


private:
    Switch* switch_ = nullptr;
    const char* filename = ".\\resources\\ground.fbx";
};

class Goal :public Gimic
{
public:
    Goal(ID3D11Device* device);
    ~Goal()override {}
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;

    bool GoalInPosition();
public:
    bool Get_GoalFlag()const { return Goal_Flag; }
 
private:
    float radius = 0.2f;
    bool Goal_Flag = false;
    const char*filename= ".\\resources\\ground.fbx";
    ID3D11Device* device;
};

class DropBox_Road :public Gimic
{
public:
    DropBox_Road(ID3D11Device*device);
    ~DropBox_Road()override {
        if (device_)
        {
            device_ = nullptr;
            delete device_;
        }
    };
public:
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
    void HitBox_TransformUpdate();
public://set
    void SetHitBox_Position(XMFLOAT3 Pos) { HitBox_Position = Pos; }
    void SetHitBox_Scale(XMFLOAT3 scale) { HitBox_Scale = scale; }
public://get
    DirectX::XMFLOAT3 GetHitBox_Position() { return HitBox_Position ; }
    DirectX::XMFLOAT3 GetHitBox_Scale() { return HitBox_Scale; }
    collision_mesh* GetHitBoxMesh()override { return HitBox->Get_RaycastCollition(); };
    XMFLOAT4X4 GetBoxTransForm()override { return HitBox_Transform; }
    bool GetPlayerStopFlag() override { return playerStopFlag; }
  
public:
    bool isPlayerInRangeOf_Box();
    struct dropboxNow
    {
        XMFLOAT3 oppnentPos{};//落ちてきた位置保持
        bool flag = false;///わたることができますよフラグ
        float scaleX;
    };
   
    dropboxNow GetDropBoxNow() { return dropbox; }
private:
    float radius = 1;
    dropboxNow dropbox;
    bool playerStopFlag{};
    XMFLOAT3 HitBox_Position{ 0.f,0.5f,0.5f };
    XMFLOAT3 HitBox_Velocty{ 0.f,0.f,0.f };
    XMFLOAT3 HitBox_Angle{ 0.f,0.f,0.f };
    XMFLOAT3 HitBox_Scale{ 200.f,100.f,10.f };
    XMFLOAT4X4 HitBox_Transform
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };
    unique_ptr<Model>HitBox;
   
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
    ID3D11Device* device_;
};