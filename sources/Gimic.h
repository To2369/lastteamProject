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
    
    //�����̃^�C�v���擾
    Gimic_Type Get_GimicType()const { return Gimic_type; }
public:
    virtual bool gimic_VS_Object() { return false; };//���̂Ƃ���switch�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    void SetSwitchFlag(bool f) { switchFlag = f; }//���̂Ƃ���switch�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    bool Get_SwitchFlag()const { return switchFlag; }//���̂Ƃ���switch�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:
    ObjType GetMyObjeFlagType()const { return MyObjeFlagType; }
    Gimic_Type GetMyGimicFlagType() { return MyGimicFlagType; }
    void SetGimicID(string id) { ID = id; }//��F�����̃X�C�b�`���o�Ă������ɂ��ꂪ�Ȃ��ƁA��̃X�C�b�`�ŃX�C�b�`�ŋN�����邷�ׂẴM�~�b�N���N�����Ă��܂�
    string GetGimicID() { return ID;}//���̊֐����g���Ď���(�M�~�b�N)���ǂ̃M�~�b�N�ɑ΂��ċN�����邩�����ʂ���
public:
  
 
    virtual collision_mesh* GetHitBoxMesh() { return nullptr; }//���̂Ƃ���DropBox�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    virtual bool GetPlayerStopFlag(){ return false; };//���̂Ƃ���DropBox�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    virtual XMFLOAT4X4 GetBoxTransForm() { return XMFLOAT4X4(); }//���̂Ƃ���DropBox�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:
    virtual void Gimic_VS_GimicFlagBoot() {};//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    bool GetBootFlag()const { return bootFlag[0]; }//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    bool GetBootFlag(int i)const { return bootFlag[i]; }//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    void SetBootFlag(bool f) { bootFlag.push_back(f); }//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:
  

protected:
    string ID;
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
        XMFLOAT3 oppnentPos{};//�����Ă����ʒu�ێ�
        bool flag = false;///�킽�邱�Ƃ��ł��܂���t���O
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