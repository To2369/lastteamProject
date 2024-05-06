#include "UIManajer.h"
#include"objectManajer.h"
using namespace DirectX;
using namespace std;
void UIManager::CreateUI(ID3D11Device* device)
{
    ObjectTypes.clear();
    GetStage_objectTypes();
    int count = ObjectTypes.size();
    
    XMFLOAT2 Scale;
    vector<unique_ptr<UI>>ui;
    for (int i = 0; i < count; i++)
    {
        wchar_t* filename=L"";
        switch (ObjectTypes[i])
        {
          case Obj_attribute::cution:
              filename = L".\\resources\\UI\\Attribute(cution).png";
              Scale = { 1280.f/11.f,720.f/11.f };
              break;
          case Obj_attribute::Super_cution:
              filename = L".\\resources\\UI\\Attribute(super_cution).png";
              Scale = { 222.f / 10.f,247.f / 10.f };
              break;
          case Obj_attribute::heavy:
              filename = L".\\resources\\UI\\Attribute(heavy).png";
              Scale = { 1280.f / 10.f,720.f / 10.f };
              break;
          case Obj_attribute::Super_heavy:
              filename = L".\\resources\\UI\\Attribute(super_heavy).png";
              Scale = { 222.f / 10.f,247.f / 10.f };
              break;
          case Obj_attribute::Fragile:
              filename = L".\\resources\\UI\\Attribute(fragile).png";
              Scale = { 1280.f / 10.f,720.f / 10.f };
              break;
          case Obj_attribute::Super_fragile:
              filename = L".\\resources\\UI\\Attribute(super_fragile).png";
              Scale = { 222.f / 10.f,247.f / 10.f };

              break;
          case Obj_attribute::Hard_to_Break:
              filename = L".\\resources\\UI\\Attribute(ŒÅ).png";
              Scale = { 222.f / 10.f,247.f / 10.f };

              break;
          case Obj_attribute::Super_hard_to_Break:
              filename = L".\\resources\\UI\\’´ŒÅ.png";
              Scale = { 222.f / 10.f,247.f / 10.f };

              break;
        }
       
        XMFLOAT2 slidPos{ 100.f * i,0.f };
        ui.push_back(make_unique<UI>(device, reinterpret_cast<const wchar_t*>(filename), Scale,slidPos));
    }
   
    unique_ptr<CanBas>can = make_unique<CanBas>(move(ui));
    CanBass.push_back(move(can));
    ui.clear();

}
void UIManager::Gui()
{
    for (auto& canbas : CanBass)
    {
        canbas->Gui();
    }
}
void UIManager::GetStage_objectTypes()
{
    Objectmanajer& ince_o = Objectmanajer::incetance();
    int count = ince_o.Get_GameObjCount();
    for (int i = 0; i < count; i++)
    {
        Object* obj = ince_o.Get_GameObject(i);
        ObjType type=obj->Get_Original_Objtype(0);
        if (type!= ObjType::null&&type!=ObjType::Crack)
        {
            ObjectTypes.push_back(type);
        }
    }

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            if (i == j)continue;
            if (ObjectTypes[i] == ObjectTypes[j])
            {
                ObjectTypes.erase(ObjectTypes.begin() + j);
            }

        }
    }
}

void UIManager::Update(float elapsedTime)
{

    for (auto& canbas : CanBass)
    {
        canbas->Update(elapsedTime);
    }
}

void UIManager::Render(RenderContext* rc)
{
    for (auto& canbas : CanBass)
    {
        canbas->Render(rc);
    }
}
