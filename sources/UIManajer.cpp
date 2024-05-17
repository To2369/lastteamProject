#include "UIManajer.h"
#include"objectManajer.h"
#include <scene_management.h>
using namespace DirectX;
using namespace std;

void UIManager::CreateCanbas()
{
    unique_ptr<CanBas>can = make_unique<CanBas>(move(UIs));
    CanBass.push_back(move(can));
    UIs.clear();
}

void UIManager::CreateCanbas(std::string id)
{
    unique_ptr<CanBas>can = make_unique<CanBas>(move(UIs), id);
    CanBass.push_back(move(can));
    UIs.clear();
}

void UIManager::CreateGameSceneUI(ID3D11Device* device)
{
    ObjectTypes.clear();
    GetStageObjectTypes();
    int count = static_cast<int>(ObjectTypes.size());

    XMFLOAT2 Scale;
    vector<unique_ptr<UI>>ui;
    for (int i = 0; i < count; i++)
    {
        wchar_t* filename = L"";
        switch (ObjectTypes[i])
        {
        case Obj_attribute::cution:
            filename = L".\\resources\\UI\\Attribute(cution).png";
            Scale = { 1280.f / 11.f,720.f / 11.f };
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
        ui.push_back(make_unique<UI>(device, reinterpret_cast<const wchar_t*>(filename), Scale, slidPos));
    }

    unique_ptr<CanBas>can = make_unique<CanBas>(move(ui), UI_StringID::CanbasID::SceneGameUI);
    CanBass.push_back(move(can));
    ui.clear();

}
void UIManager::CreateUI(ID3D11Device* device, ObjType type, vector<unique_ptr<UI>>& uis)
{
    wstring filename = L"";
    XMFLOAT2 Scale;
    switch (type)
    {
    case Obj_attribute::cution:
        filename = L".\\resources\\UI\\Attribute(cution).png";
        Scale = { 1280.f / 11.f,720.f / 11.f };
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

    unique_ptr<UI>ui;
    XMFLOAT2 pos{ 500.f,400.f };
    ui = make_unique<UI>(device, filename.c_str(), Scale, pos);
    uis.push_back(move(ui));


}
void UIManager::CreateUI(ID3D11Device* device,std::wstring filename, ObjType type, std::string id, vector<unique_ptr<UI>>& uis, DirectX::XMFLOAT2 pos)
{
   
   
    XMFLOAT2 Scale;
    unique_ptr<UI>ui;
    ui = make_unique<UI>(device, filename.c_str(), Scale, pos);
    ui->SetScale({static_cast<float>(ui->GetTexture2DDesc().Width)/3.f,static_cast<float>(ui->GetTexture2DDesc().Height)/4.f });
    ui->SetID(id);
    uis.push_back(move(ui));
}
void UIManager::Gui()
{
    for (auto& canbas : CanBass)
    {
        canbas->Gui();
    }
}

bool UIManager::Mouse_VS_UI(DirectX::XMFLOAT2 SP_POS, D3D11_TEXTURE2D_DESC& SP_desc)
{
    POINT cursor;
    GetCursorPos(&cursor);
    XMFLOAT2 cursorPos{ static_cast<float>(cursor.x),static_cast<float>(cursor.y) };
    SceneManagement& ince = SceneManagement::instance();


    //XMFLOAT2 winPos = { static_cast<float>(windowRect.left),static_cast<float>(windowRect.top) };
    XMFLOAT2 winPos = { static_cast<float>(ince.GetWindowPosition().x),static_cast<float>(ince.GetWindowPosition().y) };
    float LeftPos, RightPos, DownPos, UpPos;
    LeftPos = SP_POS.x + winPos.x;
    RightPos = SP_POS.x + SP_desc.Width + winPos.x;
    DownPos = SP_POS.y + SP_desc.Height + winPos.y + 30.f;
    UpPos = SP_POS.y + winPos.y + 30.f;
    if (cursorPos.x > LeftPos &&
        cursorPos.x < RightPos &&
        cursorPos.y<DownPos &&
        cursorPos.y>UpPos)
    {
        return true;
    }
    return false;
}

bool UIManager::Mouse_VS_UI(DirectX::XMFLOAT2 SP_POS, XMFLOAT2 Scale)
{
    POINT cursor;
    GetCursorPos(&cursor);
    XMFLOAT2 cursorPos{ static_cast<float>(cursor.x),static_cast<float>(cursor.y) };
    SceneManagement& ince = SceneManagement::instance();


    //XMFLOAT2 winPos = { static_cast<float>(windowRect.left),static_cast<float>(windowRect.top) };
    XMFLOAT2 winPos = { static_cast<float>(ince.GetWindowPosition().x),static_cast<float>(ince.GetWindowPosition().y) };
    float LeftPos, RightPos, DownPos, UpPos;
    LeftPos = SP_POS.x + winPos.x;
    RightPos = SP_POS.x + Scale.x + winPos.x + 20.f;
    DownPos = SP_POS.y + Scale.y + winPos.y + 30.f;
    UpPos = SP_POS.y + winPos.y + 30.f;
    if (cursorPos.x > LeftPos &&
        cursorPos.x < RightPos &&
        cursorPos.y<DownPos &&
        cursorPos.y>UpPos)
    {
        return true;
    }
    return false;
}
void UIManager::GetStageObjectTypes()
{
    Objectmanajer& ince_o = Objectmanajer::incetance();
    int count = ince_o.Get_GameObjCount();
    for (int i = 0; i < count; i++)
    {
        Object* obj = ince_o.Get_GameObject(i);
        ObjType type = obj->Get_Original_Objtype(0);
        if (type != ObjType::null && type != ObjType::Crack)
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
                i = 0;
                j = 0;
                count = static_cast<int>(ObjectTypes.size());
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

void UIManager::Render(RenderContext* rc, std::string id)
{
    for (auto& canbas : CanBass)
    {
        if (id == canbas->GetCanbasID())
        {
            canbas->Render(rc);
        }
    }
}

void UIManager::Render(RenderContext* rc, std::string canbas_id, std::string ui_id)
{
    for (auto& canbas : CanBass)
    {
        if (canbas_id == canbas->GetCanbasID())
        {
            canbas->Render(rc,ui_id);
        }
    }
}

