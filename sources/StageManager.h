#pragma once

#ifndef STAGE_MANAGER
#include"thread"
#include "constant_buffer.h"
#include"Stage.h"
#include"Gimic.h"
#endif//!STAGE_MANAGER

//using ObjectMap = unordered_map<StageName, unique_ptr<Object>>;
//�����͌l�I�Ɏc��������������Ȃ��ł��肢���܂�
//using Stage_ObjectSet = unordered_map<ObjType, XMFLOAT3>;
//using StageSetUp = unordered_map<StageName, Stage_ObjectSet>;


enum class DebugMode
{
    Object_Info,//object�����}�E�X�Ŏ擾
    StageSetUp,//object�̈ʒu�ݒ�
    Create_Object,//object����
    Create_Gimic,//gimic����
    Delete_Object,//object�폜
    null,
};
enum class debugType_obj_or_gimic
{
    obj,
    gimic,
    null
};
class StageManager
{
public:
    StageManager() {};
    ~StageManager() {};
    

    static StageManager& incetance()
    {
        static StageManager ince;
        return ince;
    }
    void Initialize_GameStage(StageName name,ID3D11Device* device);
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    void Rigister(unique_ptr<Stage>stage) { Stages.push_back(move(stage)); }
    void Clear();
    void Result_Object_Info(Object&obj);
    void Result_Gimic_Info(Gimic&obj);
   
    /// <summary>
    /// 
    /// </summary>
    /// <param name="name">StageManager�̒��ɂ��� GetNowStage()�������Ɏg���Ă�������</param>
    /// <returns></returns>
    Stage* GetStages(StageName name) {   for (const auto& stage : Stages) if (stage->GetNowStage() == name)return stage.get();}
   
    void Gui(ID3D11Device*device,RenderContext*rc);
    //�X�e�[�W�I�������Ƃ��ɃZ�b�g����
    void SetStage(StageName n) { s_name = n; }
    StageName GetNowStage() { return s_name; }
    void Set_CreateObject_Thred(ObjType t,ID3D11Device* device, Intersection in)/*�n���֐��̑������̏��Ԃɏ����Ă���*/
    { newObject_Thread.push_back(new thread(CreateObject,t,device,in));}
  
    void Set_CreateGimic_Thred(Gimic_Type t, ID3D11Device* device, Intersection in)
    {
        newObject_Thread.push_back(new thread(CreateGimic,t, device, in));
    }



    void DeleteThred() {
        for (auto& thred_ : newObject_Thread)
        {
            if (thred_)
            {
                thred_->join();
                delete thred_;
                thred_ = nullptr;
            }
        }
    }
private:
    
    vector<unique_ptr<Stage>>Stages;
    StageName s_name = StageName::null;

private://debug�ł����g��Ȃ��ϐ��B
    ObjType debug_type_set_pos = ObjType::null;
    DebugMode mode =DebugMode::null ;
    ObjType CreateObjeType = ObjType::null;
    Gimic_Type CreateGimicType = Gimic_Type::null;
    debugType_obj_or_gimic o_or_g;//object_or_gimic
    bool complete_createobje;
    vector<thread*> newObject_Thread;
    Intersection result_intersection{};
    Object* Debug_ParameterObj = nullptr;
    bool Object_CreateFlag = false;
    float objLength = 1.3f;//object���������ۂ�object�ʒu�̋����{��
public:
    DebugMode Getmode() { return mode; }
    ObjType GetCreateObjeType() { return CreateObjeType; }
    Gimic_Type GetCreateGimicType() { return CreateGimicType; }
    ObjType Get_debug_type() { return debug_type_set_pos; }
    
    void SetMode(DebugMode m) { mode = m; }
    void SetCreateObjeFlag(ObjType t) { CreateObjeType = t; }
    static void CreateObject(ObjType type,ID3D11Device*device, Intersection in);
    static void CreateGimic(Gimic_Type type,ID3D11Device*device, Intersection in);
    void DebugMode_MouseRayCast(DebugMode mode, ID3D11Device* device);//���̊֐��̓}�E�X�ȊO�ł̃f�o�b�O��z�肵�ĂȂ�
   
    struct scene_constants
    {
        DirectX::XMFLOAT4X4 view_projection;	//�r���[�E�v���W�F�N�V�����ϊ��s��
        DirectX::XMFLOAT4 light_direction;		//���C�g�̌���
        DirectX::XMFLOAT4 camera_position;		//�J�����ʒu
    };
    scene_constants scene_data;
public://debugmode
    void Object_Info();
    void StageSetup();
    void Create_Object(ID3D11Device* device);
    void Delete_Object();
   

};
