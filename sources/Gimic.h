#pragma once
#include"object.h"
#include"Graphics/RenderContext.h"


class Gimic :public Object
{
public:
    Gimic() {};
    virtual ~Gimic() {};

    virtual void Update(float elapsedTime) {};
    virtual void Render(RenderContext* rc) {};
   
   
public:
    void SetBootFlag(bool f) { bootFlag.push_back(f);}
    void SetSwitchFlag(bool f) { switchFlag = f; }
public:
    ObjType GetMyObjeFlagType()const { return MyObjeFlagType; }
    //�����̃^�C�v���擾
    Gimic_Type Get_GimicType()const { return Gimic_type; }
   
    //ObjType Get_GameObj() { return get_gameobj; }//�������ɏ���Ă�I�u�W�F�N�g�擾
    bool Get_SwitchFlag()const { return switchFlag; }
public://gimic�ɑ΂��ē��삷��gimic
    Gimic_Type GetMyGimicFlagType()const { return MyGimicFlagType; }
    bool GetBootFlag()const { return bootFlag[0]; }
    bool GetBootFlag(int i)const { return bootFlag[i]; }
    virtual bool gimic_VS_Object() { return false; };
    
    virtual void Gimic_VS_GimicFlagBoot() {};
    virtual collision_mesh* GetHitBoxMesh() { return nullptr; }
    virtual bool GetPlayerStopFlag() { return false; };
    virtual XMFLOAT4X4 GetBoxTransForm() { return XMFLOAT4X4(); }
protected:
    
    vector<bool> bootFlag;
    bool switchFlag=false;
    void Gimic_effect(Gimic_Type type);
    //�����̃^�C�v
    Gimic_Type Gimic_type=Gimic_Type::null;
    //ObjType get_gameobj;
    ObjType MyObjeFlagType=ObjType::null;//�������ǂ̃I�u�W�F�N�g�^�C�v�ŃM�~�b�N���N�����邩�̕ϐ�
    Gimic_Type MyGimicFlagType = Gimic_Type::null;//�������ǂ�Gimic�^�C�v�Ŏ����̃M�~�b�N���N�����邩�̕ϐ�

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

  

private:
    const char* filename = ".\\resources\\ground.fbx";
};


class Door :public Gimic
{
public:
    Door(ID3D11Device* device);
    ~Door()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gimic_VS_GimicFlagBoot()override;


public:


private:
    const char* filename = ".\\resources\\ground.fbx";
};

class Goal :public Gimic
{
public:
    Goal(ID3D11Device* device);
    ~Goal()override {}
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    bool GoalInPosition();
public:
    bool Get_GoalFlag()const { return Goal_Flag; }
 
private:
    float radius = 0.2f;
    bool Goal_Flag = false;
    const char*filename= ".\\resources\\ground.fbx";
};

class DropBox_Road :public Gimic
{
public:
    DropBox_Road(ID3D11Device*device);
    ~DropBox_Road()override {
      
    };
public:
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void HitBox_TransformUpdate();
    void SetHitBox_Position(XMFLOAT3 Pos) { HitBox_Position = Pos; }
    void SetHitBox_Scale(XMFLOAT3 scale) { HitBox_Scale = scale; }
    void SetRadius(float radius);
    collision_mesh* GetHitBoxMesh()override { return HitBox->Get_RaycastCollition(); };
    XMFLOAT4X4 GetBoxTransForm()override { return HitBox_Transform; }
    bool isPlayerInRangeOf_Box();
public:
    bool GetPlayerStopFlag() override{ return playerStopFlag; }
    struct dropboxNow
    {
        XMFLOAT3 oppnentPos{};//�����Ă����ʒu�ێ�
        bool flag = false;///�킽�邱�Ƃ��ł��܂���t���O
        float scaleX;
    };
   
    dropboxNow GetDropBoxNow() { return dropbox; }
private:
    dropboxNow dropbox;
    bool playerStopFlag;
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
};