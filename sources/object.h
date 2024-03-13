#pragma once

#include"model.h"
#include"DirectXMath.h"
#include"Graphics/RenderContext.h"
using namespace DirectX;
#include"variable_management_class_for_hit_test.h"
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

    null,
};
enum class StageName
{
    Tutolial,
    stage1_1,
    null,
};


using ObjType = Obj_attribute;
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
public:
    Model* GetModel()const { return this->model.get(); }

    XMFLOAT4X4 GetTransform()const { return Transform; }
    XMFLOAT3 GetPosition()const { return Position; }
    XMFLOAT3 GetScale() const { return Scale; }
    ObjType Get_Objtype(int Number)const { return old_attribute_state[Number]; };
    float GetReturnTimer(int i)const { return ReturnTimer[i]; }
    Intersection GetIntersection()const { return result_intersection; }
    bool GetDestroyObje()const { return DestroyObj; }
    StageName Get_MyStageName()const { return stage_name; }//���������ǂ̃X�e�[�W�ɒu����Ă邩�擾
    XMFLOAT3 GetNormal()const { return Normal; }
    bool Get_isGimic_UpPosNow()const { return isGimic_UpPosNow; }
    XMFLOAT4 InitColor()const { return { 1.f,1.f,1.f,1.f }; }//orijinalcolor
public:
    void SetReturnTimer(float timer1 = 0.f, float timer2 = 0.f) { ReturnTimer[0] = timer2; ReturnTimer[1] = timer2; }
    void SetColor(XMFLOAT4 color_) { color = color_; }
    void SetPosition(XMFLOAT3 Pos) { Position = Pos; }
    void SetScale(XMFLOAT3 scale) { Scale = scale; }
    void SetAngle(XMFLOAT3 angle) { Angle = angle; }
    void SetTransform(XMFLOAT4X4 t) { Transform = t; }
    void Set_attribute(ObjType type,int Number) { old_attribute_state[Number]=type; };
    void Set_MystageName(StageName name) { stage_name = name; }
    void Destroy() { DestroyObj = true; }
    void SetNormal(XMFLOAT3 normal) { Normal = normal; };
    void Set_isGimic_UpPosNow(bool f) { isGimic_UpPosNow = f; }
protected:
    void UpdateTransform();

    //�������͍X�V����
    void UpdateVerticalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);

    //�������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);
protected:
    //�ړ�����
    void Move(float vx, float vz, float speed);

    //���񏈗�
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //�W�����v����
    void Jump(float speed);

    //���͏����X�V
    void UpdateVelocity(float elapsedTime);

    //���n�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {}
protected:
    //�d��
    float gravity = -1.0f;
    //�n�ʂɒ��n������
    bool isGround = false;
    //���C
    float friction = 0.5f;

    //�����x
    float acceleration = 1.0f;

    //�ő呬�x
    float maxMoveSpeed = 5.0f;

    //�v�Z�p
    float moveVecX = 0.0f;
    float moveVecZ = 0.0f;

    //�󒆈ړ�
    float airControl = 0.3f;

    //�J�n�ʒu����ɂ��邽��
    float stepOffset = 1.0f;

    //�΂߂̎��Ԑ���
    float slopeRate = 1.0f;
protected:
    void box_Collition_obj();
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
    bool isGimic_UpPosNow = false; //���܎������M�~�b�N�̏�̕��̃o�E���f�B���O�{�b�N�X�ɂ��������Ă邩�ǂ���,�ƃM�~�b�N�ɑ΂��Ăł����g��Ȃ�
    bool DestroyObj = false;
    float ReturnTimer[2]{};
    StageName stage_name = StageName::null;
   vector<ObjType> original_attribute_state;
   vector<ObjType> old_attribute_state;
    Intersection result_intersection{};

    XMFLOAT3 Normal{ 0.f,0.f,0.f };//raycast�p
    XMFLOAT3 Position{ 0.f,0.f,0.f };
    XMFLOAT3 Velocty{ 0.f,0.f,0.f };
    XMFLOAT3 Angle{ 0.f,0.f,0.f };
    XMFLOAT3 Scale{ 1.f,1.f,1.f };
    XMFLOAT4 color{ 1.f,1.f,1.f,1.f };
    XMFLOAT4X4 Transform
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };
    std::unique_ptr<Model> model;
  
};