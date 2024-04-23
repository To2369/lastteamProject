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
    Delete_Object,//object�폜
    null,
};
enum class debugType
{
    obj,
    gimic,
    static_obj,
    null
};
class StageManager
{
public:
    StageManager() {};
    ~StageManager()
    {
        if (Debug_ParameterObj)
        {
            Debug_ParameterObj = nullptr;
        }
    };


    static StageManager& incetance()
    {
        static StageManager ince;
        return ince;
    }
    void Initialize_GameStage(StageName name, ID3D11Device* device);
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    void Rigister(unique_ptr<Stage>stage) { Stages.push_back(move(stage)); }
    void Clear();
    void Result_Object_Info(Object& obj);
    void Result_Gimic_Info(Gimic& obj);
    void Result_Static_Object_Info(Static_Object& obj);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="name">StageManager�̒��ɂ��� GetNowStage()�������Ɏg���Ă�������</param>
    /// <returns></returns>
    Stage* GetStages(int num) {
        return Stages[num].get();
    }
    int GetStageCount() { return Stages.size(); }
    void Gui(ID3D11Device* device, RenderContext* rc);
    //�X�e�[�W�I�������Ƃ��ɃZ�b�g����
    void SetStage(StageName n) { s_name = n; }

    void Set_CreateObject_Thred(ObjType t, ID3D11Device* device, Intersection in)/*�n���֐��̑������̏��Ԃɏ����Ă���*/
    {
        newObject_Thread.push_back(new thread(CreateObject, t, device, in));
    }

    void Set_CreateGimic_Thred(Gimic_Type t, ID3D11Device* device, Intersection in, std::string id)
    {
        newObject_Thread.push_back(new thread(CreateGimic, t, device, in, id));
    }

    void Set_CreateStaticObject_Thred(Static_ObjType t, ID3D11Device* device, Intersection in, const char* filename = nullptr)
    {
        newObject_Thread.push_back(new thread(CreateStaticObject, t, device, in, filename));
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
    ObjType SetObjType = ObjType::null;
    ObjType debug_type_set_pos = ObjType::null;
    DebugMode oldMode = DebugMode::null;
    DebugMode mode = DebugMode::null;
private:
    ObjType CreateObjeType = ObjType::null;
    Gimic_Type CreateGimicType = Gimic_Type::null;
    Static_ObjType CreateStaticObjeType = Static_ObjType::null;
    debugType o_or_g = debugType::null;//object_or_gimic
    bool complete_createobje = false;
    vector<thread*> newObject_Thread;
private:
    Intersection result_intersection{};
    Object* Debug_ParameterObj = nullptr;
    bool Object_CreateFlag = false;
    float objLength = 1.3f;//object���������ۂ�object�ʒu�̋����{��
    std::string ID = "null";
    std::string static_objFilename = "null";
public:
    DebugMode Getmode() { return mode; }
    ObjType GetCreateObjeType() { return CreateObjeType; }
    Gimic_Type GetCreateGimicType() { return CreateGimicType; }
    Static_ObjType GetCreateStaticObjeType() { return CreateStaticObjeType; }
    ObjType Get_debug_type() { return debug_type_set_pos; }

    void SetMode(DebugMode m) { mode = m; }
    void SetCreateObjeFlag(ObjType t) { CreateObjeType = t; }
    static void CreateObject(ObjType type, ID3D11Device* device, Intersection in);
    static void CreateGimic(Gimic_Type type, ID3D11Device* device, Intersection in, std::string id = "");

    static void CreateStaticObject(Static_ObjType type, ID3D11Device* device, Intersection in, const char* filename);
    void DebugMode_MouseRayCast(DebugMode mode, ID3D11Device* device);//���̊֐��̓}�E�X�ȊO�ł̃f�o�b�O��z�肵�ĂȂ�
    string GetObjectType_S(ObjType type)
    {
        switch (type)
        {
        case ObjType::cution:
            return "cushion";
            break;
        case ObjType::Super_cution:
            return "Super_cution";
            break;
        case ObjType::Hard_to_Break:
            return "Hard_to_Break";
            break;
        case ObjType::Super_hard_to_Break:
            return "Super_hard_to_Break";
            break;
        case ObjType::heavy:
            return "heavy";
            break;
        case ObjType::Super_heavy:
            return "Super_heavy";
            break;
        case ObjType::Fragile:
            return "Fragile";
            break;
        case ObjType::Super_fragile:
            return "Super_fragile";
            break;
        case ObjType::Crack:
            return "Crack";
            break;
        case ObjType::null:
            return "null";
            break;

        }

    }
    string GetGimicType_S(Gimic_Type type)
    {
        switch (type)
        {
        case Gimic_Type::Switch:
            return "Switch";

            break;
        case Gimic_Type::Door:
            return "Door";
            break;
        case Gimic_Type::Goal:
            break;
            return "Goal";
        case Gimic_Type::Drop_Road:
            return "Drop_Road";
            break;
        case Gimic_Type::null:
            return "null";
            break;
        default:
            break;
        }
        return "";
    }
    struct scene_constants
    {
        DirectX::XMFLOAT4X4 view_projection;	//�r���[�E�v���W�F�N�V�����ϊ��s��
        DirectX::XMFLOAT4 light_direction;		//���C�g�̌���
        DirectX::XMFLOAT4 camera_position;		//�J�����ʒu
    };
    scene_constants scene_data{};
public://debugmode
    void Object_Info();
    void StageSetup();
    void Create_Object(ID3D11Device* device);
    void Delete_Object();
    int filenameIndex = 0;
    const char* filenames[99]
    {
        ".\\resources\\stage1\\tana.fbx",
        nullptr
    };

};
