#pragma once
#include"object.h"
#include"Graphics/RenderContext.h"

class Gimic :public Object
{
public:
    Gimic() {};
    virtual ~Gimic() {};

    virtual void Update(float elapsedTime) = 0;
    virtual void Render(RenderContext* rc) = 0;

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
    string GetGimicID() { return ID; }//この関数を使って自分(ギミック)がどのギミックに対して起動するかを識別する
public:

    const XMFLOAT4X4 GetWallTransform() const { return BoxTransform; }
    const Model* GetWallModel() const { return Baria_Wall.get(); }//dropBoxに使ってる見えない壁

    virtual collision_mesh* GetHitBoxMesh() { return nullptr; }//今のところDropBoxクラスに対してしか使っていない
    virtual bool GetPlayerStopFlag() { return false; };//今のところDropBoxクラスに対してしか使っていない
public:
    virtual void Gimic_VS_GimicFlagBoot() {};//今のところDoorクラスに対してしか使っていない
    bool GetBootFlag()const { return bootFlag; }//今のところDoorクラスに対してしか使っていない
    void SetBootFlag(bool f) { bootFlag=f; }//今のところDoorクラスに対してしか使っていない
public:

protected:
    string ID;
    bool bootFlag;
    bool switchFlag = false;
    void Gimic_effect(Gimic_Type type);
    //自分のタイプ
    Gimic_Type Gimic_type = Gimic_Type::null;
    //ObjType get_gameobj;
    ObjType MyObjeFlagType = ObjType::null;//自分がどのオブジェクトタイプでギミックが起動するかの変数
    Gimic_Type MyGimicFlagType = Gimic_Type::null;//自分がどのGimicタイプで自分のギミックが起動するかの変数
    //dropWall二しか使っていない
    XMFLOAT3 BoxPosition{};
    XMFLOAT3 BoxScale{ 1.f,1.f,1.f };
    XMFLOAT3 BoxAngle{};
    XMFLOAT4X4 BoxTransform{
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
    };
    
    unique_ptr<Model>Baria_Wall;
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
    void FallDown();        //ドアギミックが動く時の処理
    void Gui()override;


public:


private:
    Switch* switch_ = nullptr;
    const char* filename = ".\\resources\\stage1\\syashin_tento.fbx";
    float elapsedtime_;
    float falldownCount = 0;
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
    float radius = 0.1f;
    bool Goal_Flag = false;
    const char* filename = ".\\resources\\ground.fbx";
    ID3D11Device* device;
};

class DropBox_Road :public Gimic
{
public:
    DropBox_Road(ID3D11Device* device);
    ~DropBox_Road()override {

    };
public:
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
  
private:
    bool isPlayerInRangeOf_Box();
   
    void IsWall_UpdateTransform();
    struct dropboxNow
    {
        XMFLOAT3 oppnentPos{};//落ちてきた位置保持
        bool flag = false;///わたることができますよフラグ
        float radius;
    };
  
    
   const dropboxNow GetDropBoxNow()const { return dropbox; }
private:
    
    dropboxNow dropbox;  
   
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
};