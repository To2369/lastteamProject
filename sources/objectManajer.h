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
    void Initialize_Gimic(StageName s_name_,Gimic_Type type_name,ID3D11Device*device,XMFLOAT3 pos);
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
    void Rigister_obj(std::unique_ptr<Object> obj) { game_objs.push_back(std::move(obj)); }
    void Rigister_Gimic(std::unique_ptr<Gimic> obj) { game_Gimics.push_back(std::move(obj)); }
    int Get_GameObjCount() { return game_objs.size(); }
    int Get_GameGimicCount() { return game_Gimics.size(); }
    Object* Get_GameObject(int i) { return game_objs[i].get(); }
    Gimic* Get_GameGimic(int i) { return game_Gimics[i].get(); }
    bool Bounding_Box_vs_Bounding_Box(Object* obj1, Object* obj2,bool UpcheckFlag=false, float offset=0.0f);
   
    void Gui(ID3D11Device* device);
private:
    
     vector<unique_ptr<Object>> game_objs;
     vector<unique_ptr<Gimic>> game_Gimics;
     set<Object*>remove;
   

};