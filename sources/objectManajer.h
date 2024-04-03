#pragma once
#include"d3d11.h"
#include<vector>
#include"object.h"
#include"Stage.h"
#include"Gimic.h"
class Objectmanajer
{
public:
    Objectmanajer() {}
    ~Objectmanajer() {}
    static Objectmanajer& incetance()
    {
        static Objectmanajer ince;
        return ince;
    }
    void Initialize_Obj(StageName s_name_,ObjType type_name,ID3D11Device*device,XMFLOAT3 pos);
    void Initialize_Gimic(StageName s_name_,Gimic_Type type_name,ID3D11Device*device,XMFLOAT3 pos,std::string id="null");
    void Update(float elapsedTime);
    void render(RenderContext* rc);
   
    void Clear();
    void Set_eraceObgect(Object& obj) { 
        auto destroyPredicate = [&](const unique_ptr<Object>& element) {
            return element.get() == &obj && obj.GetDestroyObje();
            };
        auto it = remove_if(game_objs.begin(), game_objs.end(), destroyPredicate);
        game_objs.erase(it, game_objs.end()); 
        
       }
    void Set_eraceObgect(Gimic& obj) {
        auto destroyPredicate = [&](const unique_ptr<Gimic>& element) {
            return element.get() == &obj && obj.GetDestroyObje();
            };
        auto it = remove_if(game_Gimics.begin(), game_Gimics.end(), destroyPredicate);
        game_Gimics.erase(it, game_Gimics.end());

    }
    //この関数を使うときはそのオブジェクトがステージに存在してるのが1つの時だけ
    Gimic* Select_GetGimic(Gimic_Type type)
    {
        int count = static_cast<int>(game_Gimics.size());
        Gimic* gi=nullptr;
        int type_count =0;
        for (int i = 0; i < count; i++)
        {
            gi = game_Gimics[i].get();
            if (gi->Get_GimicType() == type)
            {
                type_count++;
                if (type_count >= 2)return nullptr;
            
            }
        }
        return gi;
    };
    void Rigister_obj(std::unique_ptr<Object> obj) { game_objs.push_back(std::move(obj)); }
    void Rigister_Gimic(std::unique_ptr<Gimic> obj) { game_Gimics.push_back(std::move(obj)); }
    int Get_GameObjCount() { return static_cast<int>(game_objs.size()); }
    int Get_GameGimicCount() { return static_cast<int>(game_Gimics.size()); }
    Object* Get_GameObject(int i) { return game_objs[i].get(); }
    Gimic* Get_GameGimic(int i) { return game_Gimics[i].get(); }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="thisobj">自分がgimicの上に乗ってるかチェックを行うオブジェクト</param>
    /// <param name="gimic"></param>
    /// <param name="UpcheckFlag"></param>
    /// <param name="offset"></param>
    /// <returns></returns>
    bool Bounding_Box_vs_Bounding_Box(Object* obj, Object* gimic_obj,bool UpcheckFlag=false, float offset=0.0f);

    bool Bounding_Box_vs_Bounding_Box(Object* obj, Object* gimic_obj,DirectX::XMFLOAT3 vel);
   
    void Gui(ID3D11Device* device);
private:
    
     vector<unique_ptr<Object>> game_objs;
     vector<unique_ptr<Gimic>>  game_Gimics;
     
     set<Object*>remove;
   

};