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
    string GetGimicID() { return ID; }//���̊֐����g���Ď���(�M�~�b�N)���ǂ̃M�~�b�N�ɑ΂��ċN�����邩�����ʂ���
public:

    const XMFLOAT4X4 GetWallTransform() const { return BoxTransform; }
    const Model* GetWallModel() const { return Baria_Wall.get(); }//dropBox�Ɏg���Ă錩���Ȃ���

    virtual collision_mesh* GetHitBoxMesh() { return nullptr; }//���̂Ƃ���DropBox�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    virtual bool GetPlayerStopFlag() { return false; };//���̂Ƃ���DropBox�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:
    virtual void Gimic_VS_GimicFlagBoot() {};//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    bool GetBootFlag()const { return bootFlag; }//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    void SetBootFlag(bool f) { bootFlag=f; }//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:

protected:
    string ID;
    bool bootFlag;
    bool switchFlag = false;
    void Gimic_effect(Gimic_Type type);
    //�����̃^�C�v
    Gimic_Type Gimic_type = Gimic_Type::null;
    //ObjType get_gameobj;
    ObjType MyObjeFlagType = ObjType::null;//�������ǂ̃I�u�W�F�N�g�^�C�v�ŃM�~�b�N���N�����邩�̕ϐ�
    Gimic_Type MyGimicFlagType = Gimic_Type::null;//�������ǂ�Gimic�^�C�v�Ŏ����̃M�~�b�N���N�����邩�̕ϐ�
    //dropWall�񂵂��g���Ă��Ȃ�
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
    void FallDown();        //�h�A�M�~�b�N���������̏���
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
        XMFLOAT3 oppnentPos{};//�����Ă����ʒu�ێ�
        bool flag = false;///�킽�邱�Ƃ��ł��܂���t���O
        float radius;
    };
  
    
   const dropboxNow GetDropBoxNow()const { return dropbox; }
private:
    
    dropboxNow dropbox;  
   
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
};