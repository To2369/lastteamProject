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
    //�����̃^�C�v���擾
    Gimic_Type Get_GimicType()const { return Gimic_type; }
    bool RayCastFlag() { return Raycastflag; }
public:
    virtual bool gimic_VS_Object() { return false; };//���̂Ƃ���switch�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    void SetSwitchFlag(bool f) { switchFlag = f; }//���̂Ƃ���switch�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    bool Get_SwitchFlag()const { return switchFlag; }//���̂Ƃ���switch�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:
    ObjType GetMyObjeFlagType()const { return MyObjeFlagType; }
    Gimic_Type GetMyGimicFlagType() { return MyGimicFlagType; }
    void SetMyObjeFlagtype(ObjType type) { MyObjeFlagType = type; }//����(switch)�ɏ���ăM�~�b�N���N�������鐫����ݒ�
    void SetGimicID(string id) { ID = id; }//��F�����̃X�C�b�`���o�Ă������ɂ��ꂪ�Ȃ��ƁA��̃X�C�b�`�ŃX�C�b�`�ŋN�����邷�ׂẴM�~�b�N���N�����Ă��܂�
    string GetGimicID() { return ID; }//���̊֐����g���Ď���(�M�~�b�N)���ǂ̃M�~�b�N�ɑ΂��ċN�����邩�����ʂ���
public:
   
    const XMFLOAT4X4 GetWallTransform() const { return BoxTransform; }
    const Model* GetWallModel() const { return Baria_Wall.get(); }//dropBox�Ɏg���Ă錩���Ȃ���
    void IsWall_UpdateTransform();

    virtual collision_mesh* GetHitBoxMesh() { return nullptr; }//���̂Ƃ���DropBox�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    virtual bool GetPlayerStopFlag() { return false; };//���̂Ƃ���DropBox�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:
    virtual void Gimic_VS_GimicFlagBoot() {};//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    bool GetBootFlag()const { return bootFlag; }//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
    void SetBootFlag(bool f) { bootFlag=f; }//���̂Ƃ���Door�N���X�ɑ΂��Ă����g���Ă��Ȃ�
public:
    bool Get_GoalFlag()const { return Goal_Flag; }//goal�ɂ����g���Ă��Ȃ�
public:
    //lift�ɂ����g��Ȃ�
    enum class LiftType
    {
        LEFT,
        RIGHT,
        null
    };
    XMFLOAT3 StartPos = {};
    XMFLOAT3 EndPos = {};
    void SetEndPos(XMFLOAT3 p) { EndPos = p; }
    //lift�ȊO�Ŏg��Ȃ�
    LiftType GetLiftType()const { return type_; }
    void SetLiftType(LiftType type) { type_ = type; }
    void SetisLift(ObjType type) { isLift_Objtype = type; };
    ObjType GetIsLift() { return isLift_Objtype; }//lift�ɏ���Ă�I�u�W�F�N�g��type�擾
    ObjType isLift_Objtype = ObjType::null;
protected:
    LiftType type_=LiftType::null;
    string ID;
    bool Raycastflag = false;
    bool bootFlag;
    bool switchFlag = false;
    bool Goal_Flag = false;
    void Gimic_effect(Gimic_Type type);
    //�h�A�M�~�b�N���������̏���(��]�������p�x,��]���x)
    void FallDown(float MaxAngle,float speed);
    float falldownAngle = 0;    //FallDown�֐��Ŏg���Ă�
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
        XMFLOAT3 oppnentPos{};//�����Ă����ʒu�ێ�
        bool flag = false;///�킽�邱�Ƃ��ł��܂���t���O
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
    XMFLOAT4X4 render_WorldTransform{};//�����_�[�p�ƕۑ��p
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