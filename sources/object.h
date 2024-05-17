#pragma once

#include"model.h"

#include"Graphics/RenderContext.h"
#include"Collision.h"
#include"Mathf.h"

enum class Obj_attribute
{
    cution,//�y��//�����
    Super_cution,//���y��
    heavy,//�d��//�����
    Super_heavy,//���d��

    Fragile,//���₷��
    Super_fragile,//�����₷��

    Hard_to_Break,//���ɂ���
    Super_hard_to_Break,//�����ɂ���

    Crack,//�Ђъ���

    null,
};
enum class StageName
{
  
    stage1_1=0,
    stage1_2,
    stage1_3,
    null=-1,
};
enum class Gimic_Type
{
    Switch,
    Door,
    Goal,
    Drop_Road,
    Lift,
    null
};
//Gui���}�W�b�N�i���o�[���g�p���đ��삵�Ȃ��悤
struct Gui_parameter_Valu
{
    const DirectX::XMFLOAT3 Max{ 0.5f,0.5f,0.5f };
    const DirectX::XMFLOAT3 Min{ -0.5f,-0.5f,-0.5f };
};
using ObjType = Obj_attribute;


//
//struct FaceBoundingBoxCollition//�e���_���v�Z�������������߂Ă��̍\���̂ɏ�����������
//{
//    enum class face
//    {
//        Right,
//        Left,
//        Front,
//        Back,
//        null
//    };
//    face type;
//    float length;
//
//};
//�Q�[���Ŏg���u���̊��N���X
class Object
{
public:
    Object()
    {

    };
    virtual ~Object() {};
    virtual void Update(float elapsedTime) = 0;
    virtual void Render(RenderContext* rc) = 0;
private:
    float animation_tick{ 0 };
public:
    void BaseGui();
    virtual void Gui() {};
public:
    Model* GetModel()const { return this->model.get(); }

    DirectX::XMFLOAT4X4 GetTransform()const { return Transform; }
    DirectX::XMFLOAT3 GetPosition()const { return Position; }
    DirectX::XMFLOAT3 GetScale() const { return Scale; }
    ObjType Get_Old_Objtype(int Number)const { return old_attribute_state[Number]; };//���̊֐������A���^�C���ōX�V�����Type
    ObjType Get_Original_Objtype(int Number)const { return original_attribute_state[Number]; };//�萔Type//��{s�I�ɂO�Ԗڂ��Ƃ�
    float GetReturnTimer(int i)const { return ReturnTimer[i]; }
    bool GetDestroyObje()const { return DestroyObj; }
    StageName Get_MyStageName()const { return stage_name; }//���������ǂ̃X�e�[�W�ɒu����Ă邩�擾
    //DirectX::XMFLOAT3 GetNormal()const { return Normal; }
    bool Get_isGimic_UpPosNow()const { return isGimic_UpPosNow; }
    //���������N�����Ă�ID���擾
    std::string Get_BootGimicType()const { return GetBootGimicType_ID; }

    float GetRadius()const { return radius; }
    bool GetMoveObjectFlag()const { return moveobjectFlag; };
    bool GetIsWall() const{ return isWall;}
    bool GetIsObject()const { return isObject; }
    bool GetStatic_Objflag() { return static_ObjFlag; }
public:
    void InvisibleWall_VS_Object();
    void SetMoveObjectFlag(bool f) { moveobjectFlag = f; }
    void SetVelotyXZ(DirectX::XMFLOAT2 sp) { VelocityXZ = sp; }
    void SetReturnTimer(float timer1 = 0.f, int num = 0) { ReturnTimer[num] = timer1; }
    void SetColor(DirectX::XMFLOAT4 color_) { color = color_; }
    void SetPosition(XMFLOAT3 Pos) { Position = Pos; }
    void SetScale(DirectX::XMFLOAT3 scale) { Scale = scale; }
    void SetAngle(DirectX::XMFLOAT3 angle) { Angle = angle; }
    void SetTransform(DirectX::XMFLOAT4X4 t) { Transform = t; }
    void Set_attribute(ObjType type, int Number) {
        old_attribute_state[Number] = type;
    };
    void Set_MystageName(StageName name) { stage_name = name; }
    void Destroy() { DestroyObj = true; }//�I�u�W�F�N�g�j�����邽�߂̊֐�
    void SetNormal(DirectX::XMFLOAT3 normal) { Normal = normal; };
    void Set_isGimic_UpPosNow(bool f) { isGimic_UpPosNow = f; }
    void Set_GimicType(string g) { GetBootGimicType_ID = g; }
    void SetIsWall(bool f) { isWall = f; }
    void SetStatic_Objflag(bool f) { static_ObjFlag = f; }
public:
    enum class SphereAttribute
    {
        Right,
        Left,
        Front,
        Backfront,
        null
    };
    struct SphereQuadPlacement
    {
        struct sphereType
        {
            SphereAttribute Type = SphereAttribute::null;
            XMFLOAT3 Spherepos{};
        };
        SphereQuadPlacement() {};
        ~SphereQuadPlacement() {};
        SphereQuadPlacement(XMFLOAT3 thisPos)
        {
            pos[0].Spherepos = pos[1].Spherepos = pos[2].Spherepos = pos[3].Spherepos = thisPos;
            pos[0].Type = SphereAttribute::Right;
            pos[1].Type = SphereAttribute::Left;
            pos[2].Type = SphereAttribute::Front;
            pos[3].Type = SphereAttribute::Backfront;
        };
        float sphereLength = 0;
        float SphereRadius = 0.0123f;
        sphereType pos[4]{};
        int size = sizeof(pos) / sizeof(pos[0]);
    };
    struct ResultSphereQuadPlacement
    {
        SphereAttribute type = SphereAttribute::null;
        XMFLOAT3 Spherepos;
    };
    SphereQuadPlacement Mysphere{};
    ResultSphereQuadPlacement ResultSphere{};
    //SphereQuadPlacement�𐶐�������ɂ��̊֐��ɓn��
    void SetMySphere(const SphereQuadPlacement& sphere) { Mysphere = sphere; }
    const SphereQuadPlacement GetMySphere()const { return Mysphere; }
public:
    struct RayCastList//������g����raycast���邩�ǂ����𐧌䂷��
    {
        bool IsRayCastGround = false;
        bool IsRayCastGimic = false;
        bool IsRayCastObject = false;
        bool IsRayCastInvisibleWall = false;
    }israycast;
protected:

   // bool QuadPlacement_vs_ThisSphere(const SphereQuadPlacement& sphere, const DirectX::XMFLOAT3& Position);

    void CreateQuadPlacement(SphereQuadPlacement& sphere);

    virtual void UpdateTransform();
    void RayCastGround();
    //�����̌���
    void ObjType_effect(float elapsedTime);
    //�����������Ă����I���W�i���̐����ɖ߂��֐�
    void Return_orijinal_ObjType(float elapsedTime);
    //���̊֐��̓R���X�g���N�^�ȊO�Ŏg��Ȃ�
    void initialaize_Set_attribute(ObjType type1, ObjType type2)
    {
        original_attribute_state.push_back(type1);
        original_attribute_state.push_back(type2);
        old_attribute_state.push_back(type1);
        old_attribute_state.push_back(type2);
    }

    bool isObject = false;
    bool moveobjectFlag = false;
    float radius = 0.123f;
    XMFLOAT3 oldPosition = {};//�ʒu�X�V����O�ɂ��̕ϐ��ɑ��
    DirectX::XMFLOAT3 Normal{ 0.f,0.f,0.f };//raycast�p
    DirectX::XMFLOAT3 Position{ 0.f,0.f,0.f };
    DirectX::XMFLOAT2 VelocityXZ = { 0.0f,0.0f };
    float VeloctyY = 0;
    DirectX::XMFLOAT3 Angle{ 0.f,0.f,0.f };
    DirectX::XMFLOAT3 Scale{ 1.f,1.f,1.f };
    DirectX::XMFLOAT4 color{ 1.f,1.f,1.f,1.f };
    DirectX::XMFLOAT4X4 Transform
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };
    std::unique_ptr<Model> model;
private:
  
    bool static_ObjFlag = false;
    bool isWall = false;
    std::string GetBootGimicType_ID = "null";
    bool isGimic_UpPosNow = false; //���܎������M�~�b�N�̏�̕��̃o�E���f�B���O�{�b�N�X�ɂ��������Ă邩�ǂ���,�M�~�b�N�ɑ΂��Ăł����g��Ȃ�
    bool DestroyObj = false;
    float ReturnTimer[2]{};
    StageName stage_name = StageName::null;
    //todo:������vector�ɂ���K�v���Ȃ��̂Ŏ肪�󂫎���z��ɏ���������
    vector<ObjType> original_attribute_state;
    vector<ObjType> old_attribute_state;
    Intersection result_intersection{};

protected://gui
    DirectX::XMFLOAT4 InitColor()const { return { 1.f,1.f,1.f,1.f }; }//orijinalcolor
    Gui_parameter_Valu parameter_valu{};
    ResultSphereQuadPlacement resultsphere_GUI;
};