#pragma once
#include"d3d11.h"
#include<vector>
#include"object.h"

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
    
    void Update(float elapsedTime);
    void render(ID3D11DeviceContext* dc);
   
    void Clear();
  
    void Rigister_obj(std::unique_ptr<Object> obj) { game_objs.push_back(std::move(obj)); }
    void Remove_obj(Object*obj);
    int Get_GameObjCount() { return game_objs.size(); }
    Object* Get_GameObject(int i) { return game_objs.at(i).get(); }
    
    void Gui();
private:
     vector<unique_ptr<Object>> game_objs;
    set<Object*>remove;


};