#include"objectManajer.h"
#include "../imgui/imgui.h"
using namespace std;

void Objectmanajer::Update(float elapsedTime)
{
 
    int count = game_objs.size();
    for (int i=0;i<count;i++)
    {
        game_objs[i]->Update(elapsedTime);
    }

#if false
    for (Object*obj : remove)
    {
        auto it = find(game_objs.begin(), game_objs.end(), obj);
        if (it != game_objs.end())
            game_objs.erase(it);
        delete obj;
    }
#endif


    // 破棄リストをクリア
    remove.clear();

}

void Objectmanajer::render(ID3D11DeviceContext* dc)
{
    int count = game_objs.size();
    for (int i = 0; i < count; i++)
    {
        game_objs[i]->Render(dc);
    }
}

void Objectmanajer::Clear()
{
   
    game_objs.clear();
}

void Objectmanajer::Remove_obj(Object*obj)
{
    remove.insert(obj);
}

void Objectmanajer::Gui()
{
	ImGui::Begin("ObjectManager");


	
	ImGui::End();

}
