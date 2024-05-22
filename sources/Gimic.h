#pragma once
#include"object.h"
#include"Graphics/RenderContext.h"
#include"audio.h"
class Gimic :public Object
{
public:
    Gimic() {};
    virtual ~Gimic() {};

    virtual void Update(float elapsedTime) = 0;
    virtual void Render(RenderContext* rc) = 0;
    virtual bool Raycast(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, HitResult& Hit);
    virtual void ResultInfo();
    //自分のタイプを取得
    Gimic_Type Get_GimicType()const { return Gimic_type; }
    bool RayCastFlag() { return Raycastflag; }
public:
    virtual bool gimic_VS_Object() { return false; };//今のところswitchクラスに対してしか使っていない
    void SetSwitchFlag(bool f) { switchFlag = f; }//今のところswitchクラスに対してしか使っていない
    bool Get_SwitchFlag()const { return switchFlag; }//今のところswitchクラスに対してしか使っていない
public:
    ObjType GetMyObjeFlagType()const { return MyObjeFlagType; }
    Gimic_Type GetMyGimicFlagType() { return MyGimicFlagType; }
    void SetMyObjeFlagtype(ObjType type) { MyObjeFlagType = type; }//自分(switch)に乗ってギミックを起動させる性質を設定
    void SetGimicID(string id) { ID = id; }//例：複数のスイッチが出てきた時にこれがないと、一つのスイッチでスイッチで起動するすべてのギミックが起動してしまう
    string GetGimicID() { return ID; }//この関数を使って自分(ギミック)がどのギミックに対して起動するかを識別する
public:
   
    const XMFLOAT4X4 GetWallTransform() const { return BoxTransform; }
    const Model* GetWallModel() const { return Baria_Wall.get(); }//dropBoxに使ってる見えない壁
    void IsWall_UpdateTransform();

    virtual collision_mesh* GetHitBoxMesh() { return nullptr; }//今のところDropBoxクラスに対してしか使っていない
    virtual bool GetPlayerStopFlag() { return false; };//今のところDropBoxクラスに対してしか使っていない
public:
    virtual void Gimic_VS_GimicFlagBoot() {};//今のところDoorクラスに対してしか使っていない
    bool GetBootFlag()const { return bootFlag; }//今のところDoorクラスに対してしか使っていない
    void SetBootFlag(bool f) { bootFlag=f; }//今のところDoorクラスに対してしか使っていない
public:
    bool Get_GoalFlag()const { return Goal_Flag; }//goalにしか使っていない
public:
    //liftにしか使わない
    enum class LiftType
    {
        LEFT,
        RIGHT,
        null
    };
    XMFLOAT3 StartPos = {};
    XMFLOAT3 EndPos = {};
    void SetEndPos(XMFLOAT3 p) { EndPos = p; }
    //lift以外で使わない
    LiftType GetLiftType()const { return type_; }
    void SetLiftType(LiftType type) { type_ = type; }
    void SetisLift(ObjType type) { isLift_Objtype = type; };
    ObjType GetIsLift() { return isLift_Objtype; }//liftに乗ってるオブジェクトのtype取得
    ObjType isLift_Objtype = ObjType::null;
protected:
    LiftType type_=LiftType::null;
    string ID;
    bool Raycastflag = false;
    bool bootFlag;
    bool switchFlag = false;
    bool Goal_Flag = false;
    void Gimic_effect(Gimic_Type type);
    //ドアギミックが動く時の処理(回転したい角度,回転速度)
    void FallDown(float MaxAngle,float speed);
    float falldownAngle = 0;    //FallDown関数で使ってる
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
    Switch(ID3D11Device* device,const char*filename_);
    ~Switch()override {

    };
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    bool gimic_VS_Object()override;
    void Gui()override;
    bool flag_se=false;

private:
    const char* filename = ".\\resources\\stage1\\bottan.fbx";
};


class Door :public Gimic
{
public:
    Door(ID3D11Device* device);
    Door(ID3D11Device* device,const char* filename_);
    ~Door()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    bool Raycast(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, HitResult& Hit)override;

    void Gimic_VS_GimicFlagBoot()override;
    void Gui()override;
  
public:


private:
   
    const char* filename = ".\\resources\\stage1\\syashintate.fbx";
    float elapsedtime_;
    float falldownCount = 0;
};

class Goal :public Gimic
{
public:
    Goal(ID3D11Device* device);
    Goal(ID3D11Device* device, const char* filename_);
    ~Goal()override {}
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
    
    bool GoalInPosition();
private:
    float radius = 0.1f;
    const char* filename = ".\\resources\\Goal\\goal.fbx";
  
};

class DropBox_Road :public Gimic
{
public:
    DropBox_Road(ID3D11Device* device);
    DropBox_Road(ID3D11Device* device,const char* filename_);
    ~DropBox_Road()override {

    };
public:
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
    bool Raycast(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, HitResult& Hit)override;
   
private:
    bool isPlayerInRangeOf_Box();
   
  
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

class Lift :public Gimic
{
public:
    Lift(ID3D11Device* device,XMFLOAT3 pos_);
    Lift(ID3D11Device* device,XMFLOAT3 pos_,const char* filename_);
    ~Lift() {
       
    };
public:
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
    bool Raycast(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, HitResult& Hit)override;
    void ResultInfo()override;
    void IdentityMatrix();
private:
    XMFLOAT4X4 render_WorldTransform{};//レンダー用と保存用
    XMFLOAT4X4 oldWorldTransform{1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,0,1 };;
  
    const char* filename = ".\\resources\\stage3\\rihuto.fbx";
    const char* box_filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
    float moveRate = 0.0f;
    float moveSpeed = 1.360f;

    vector<Model::mesh>meshes;

};

class Move_Object :public Gimic
{
public:
    Move_Object(ID3D11Device* device, DirectX::XMFLOAT3 pos,bool flags[3],float movespeed=0);
    Move_Object(ID3D11Device* device,const char* filename_,DirectX::XMFLOAT3 pos,bool flags[3],float movespeed=0);
    ~Move_Object();
    void Gui()override;
public:
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
private:
  
    float moveSpeed=0.5f;
    bool x_flag = false;
    bool y_flag = false;
    bool z_flag = false;
    const char* filename=".\\resources\\stage1\\tumiki.fbx";
    
};