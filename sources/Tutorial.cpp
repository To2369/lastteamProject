#include "Tutorial.h"
#include"UIManajer.h"
#include"PlayerManager.h"
#include"objectManajer.h"
using namespace std;
using namespace DirectX;
Tutorial::Tutorial()
{
    vector<unique_ptr<UI>> uis;
    unique_ptr<UI>ui;
    UIManager& ince_ui = UIManager::incetance();
    Graphics& gr = Graphics::Instance();
    canbassid = "Tutorial";
    wstring str;
    //uiê∂ê¨
    {
        str = failepath::UI_Instructions_path_Wstring + L"camera.png";
        ui = make_unique<UI>(gr.GetDevice(),str.c_str());
        
        ui->SetID(UI_StringID::UI_ID::Tutorial_ID::Instruction_Camera);
        uiid.push_back(ui->GetID());
        ui->SetHanteiFlag(false);
        ui->SetPosition({ -426,360 });
        uis.push_back(move(ui));
        ui = nullptr;
        str = failepath::UI_Instructions_path_Wstring + L"extraction.png";
        ui = make_unique<UI>(gr.GetDevice(),str.c_str());
        ui->SetID(UI_StringID::UI_ID::Tutorial_ID::Instruction_Extraction);
        uiid.push_back(ui->GetID());
        ui->SetPosition({ -426,360 });
        ui->SetHanteiFlag(false);
        uis.push_back(move(ui));
        ui = nullptr;
        str = failepath::UI_Instructions_path_Wstring + L"injection.png";
        ui = make_unique<UI>(gr.GetDevice(), str.c_str());
        ui->SetID(UI_StringID::UI_ID::Tutorial_ID::Instruction_Injection);
        uiid.push_back(ui->GetID());
        ui->SetPosition({ -426,360 });
        ui->SetHanteiFlag(false);
        uis.push_back(move(ui));
        ui = nullptr;
        str = failepath::UI_Instructions_path_Wstring + L"jump.png";
        ui = make_unique<UI>(gr.GetDevice(), str.c_str());
        ui->SetID(UI_StringID::UI_ID::Tutorial_ID::Instruction_Jump);
        uiid.push_back(ui->GetID());
        ui->SetPosition({ -426,360 });
        ui->SetHanteiFlag(false);
        uis.push_back(move(ui));
        ui = nullptr;
        str = failepath::UI_Instructions_path_Wstring + L"move.png";
        ui = make_unique<UI>(gr.GetDevice(),str.c_str());
        ui->SetID(UI_StringID::UI_ID::Tutorial_ID::Instruction_Move);
        uiid.push_back(ui->GetID());
        ui->SetPosition({ -426,360 });
        ui->SetHanteiFlag(false);
        uis.push_back(move(ui));
        ui = nullptr;
        str = failepath::UI_Instructions_path_Wstring + L"push.png";
        ui = make_unique<UI>(gr.GetDevice(),str.c_str());
        ui->SetID(UI_StringID::UI_ID::Tutorial_ID::Instruction_Push);
        uiid.push_back(ui->GetID());
        ui->SetPosition({-426,360 });
        ui->SetHanteiFlag(false);
        uis.push_back(move(ui));
        ince_ui.UiVector_Pointer_move(move(uis));
        ince_ui.CreateCanbas(canbassid);
    }
}

Tutorial::~Tutorial()
{
    sphere.clear();
}

void Tutorial::Update(float elapsedTime)
{
    
    UIManager& ince_ui = UIManager::incetance();
    Objectmanajer& ince = Objectmanajer::incetance();
    bool gimicflag = false;
    for (int i = 0; i < ince.Get_GameGimicCount(); i++)
    {
        Gimic* gimic = ince.Get_GameGimic(i);
        if (gimic->Get_GimicType() != Gimic_Type::Switch)continue;
        if (gimic->Get_SwitchFlag())
        {
            gimicflag = true;
        }
    }


    for (auto& id : sphere)
    {
        bool moveflag[3];
        TutorialSphere T_shpere;
        bool chengetutorial = false;
        bool a = false;
        XMFLOAT2 startpos = { 0.f,460.f };
        XMFLOAT2 endpos = { -426.f,460.f };
        
       

       
        if (id->name_ == Tutorial_MapName::Instruction_Push&&id->tutorialflag&&gimicflag)
        {
            moveflag[0] = false;
            moveflag[1] = false;
            moveflag[2] = false;
            
            id->tutorialcount = 1;
            ince_ui.Update_Move_UI(elapsedTime, canbassid, id->uiid, endpos, moveflag, 0.1f);
        }
        if (id->tutorialflag&&id->name_!=nowspheretype&& id->name_ != Tutorial_MapName::Instruction_Push)
        {

            moveflag[0] = false;
            moveflag[1] = false;
            moveflag[2] = false;
            id->tutorialcount = 1;
            ince_ui.Update_Move_UI(elapsedTime, canbassid, id->uiid, endpos, moveflag, 0.1f);

        }
        if (TutorialSphereVsPlayer(*id)&&id->tutorialcount<=0)
        {
            moveflag[0] = true;
            moveflag[1] = false;
            moveflag[2] = false;
            nowspheretype = id->name_;
            if (ince_ui.Update_Move_UI(elapsedTime, canbassid, id->uiid, startpos, moveflag, 0.1f))
            {
                id->tutorialflag = true;

            }
            
             
            break;
        }
       
       
    }
}

void Tutorial::Render(Graphics& gr)
{
    UIManager& ince_ui = UIManager::incetance();
    ince_ui.GetCanbas(canbassid)->Render(gr);
}

bool Tutorial::TutorialSphereVsPlayer(TutorialSphere& sphere_)
{
    if (sphere_.tutorialcount > 0)return false;
    Player* pl = PlayerManager::Instance().GetPlayer(0);
  
    XMVECTOR Len = XMVector3Length(XMVectorSubtract(XMLoadFloat3(&sphere_.Position), XMLoadFloat3(&pl->GetPosition())));
    float len = XMVectorGetX(Len);
    float rad = sphere_.radius + pl->getRadius();
    if (len < rad)
    {

        return true;
    }

    return false;
}
