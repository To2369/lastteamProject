#pragma once

#ifndef STAGE_MANAGER
#include"thread"
#include "constant_buffer.h"
#include"Stage.h"
#include"Gimic.h"
#endif//!STAGE_MANAGER

//using ObjectMap = unordered_map<StageName, unique_ptr<Object>>;
//ここは個人的に残したいから消さないでお願いします
//using Stage_ObjectSet = unordered_map<ObjType, XMFLOAT3>;
//using StageSetUp = unordered_map<StageName, Stage_ObjectSet>;


enum class DebugMode
{
    Object_Info,//object情報をマウスで取得
    StageSetUp,//objectの位置設定
    Create_Object,//object生成
    Create_Gimic,//gimic生成
    Delete_Object,//object削除
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
    /// <param name="name">StageManagerの中にある GetNowStage()を引数に使ってください</param>
    /// <returns></returns>
    Stage* GetStages(StageName name) {   for (const auto& stage : Stages) if (stage->GetNowStage() == name)return stage.get();}
   
    void Gui(ID3D11Device*device,RenderContext*rc);
    //ステージ選択したときにセットする
    void SetStage(StageName n) { s_name = n; }
    StageName GetNowStage() { return s_name; }
    void Set_CreateObject_Thred(ObjType t,ID3D11Device* device, Intersection in)/*渡す関数の第一引数の順番に書いていく*/
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

private://debugでしか使わない変数達
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
    float objLength = 1.3f;//objectを持った際のobject位置の距離倍率
public:
    DebugMode Getmode() { return mode; }
    ObjType GetCreateObjeType() { return CreateObjeType; }
    Gimic_Type GetCreateGimicType() { return CreateGimicType; }
    ObjType Get_debug_type() { return debug_type_set_pos; }
    
    void SetMode(DebugMode m) { mode = m; }
    void SetCreateObjeFlag(ObjType t) { CreateObjeType = t; }
    static void CreateObject(ObjType type,ID3D11Device*device, Intersection in);
    static void CreateGimic(Gimic_Type type,ID3D11Device*device, Intersection in);
    void DebugMode_MouseRayCast(DebugMode mode, ID3D11Device* device);//この関数はマウス以外でのデバッグを想定してない
   
    struct scene_constants
    {
        DirectX::XMFLOAT4X4 view_projection;	//ビュー・プロジェクション変換行列
        DirectX::XMFLOAT4 light_direction;		//ライトの向き
        DirectX::XMFLOAT4 camera_position;		//カメラ位置
    };
    scene_constants scene_data;
public://debugmode
    void Object_Info();
    void StageSetup();
    void Create_Object(ID3D11Device* device);
    void Delete_Object();
   

};
