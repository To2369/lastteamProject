#pragma once
#include"Graphics/sprite.h"
#include"Graphics/graphics.h"
#include"memory"
#include <map>
#include"vector"
#include"DirectXMath.h"
class Tutorial
{
public:
    Tutorial();
    ~Tutorial();
    void Update(float elapsedTime);
    void Render(Graphics&gr);
    
public:
    enum class Tutorial_MapName :int
    {
        Instruction_Camera,
        Instruction_Move,
        Instruction_Jump,
        Instruction_Extraction,
        Instruction_Injection,
        Instruction_Push,
        null
    };
    struct TutorialSphere
    {
        TutorialSphere() {};
        TutorialSphere(bool f[3], std::string id, Tutorial_MapName name, DirectX::XMFLOAT3 pos = {})
        {
            moveflag[0] = f[0];
            moveflag[1] = f[1];
            moveflag[2] = f[2];
            Position = pos;
            uiid = id;
            name_ = name;
        }
        TutorialSphere(bool f[3],std::string id, Tutorial_MapName name,DirectX::XMFLOAT3 pos,float rad)
        {
            moveflag[0] = f[0];
            moveflag[1] = f[1];
            moveflag[2] = f[2];
            Position = pos;
            uiid = id;
            name_ = name;
            radius = rad;
        }
        Tutorial_MapName name_ = Tutorial_MapName::null;
        float radius = 0.5f;
        DirectX::XMFLOAT3 Position = {};
        bool moveflag[3] = {};
        std::string uiid;
        bool loopf = false;
        int  tutorialcount=0;//Ž©•ª‚ª‰½‰ñŒÄ‚Î‚ê‚½‚©
        bool tutorialflag=false;//
    };
    TutorialSphere* GetTutorialSphere(int i) { return sphere[i].get(); }
    int GetTutorialSphereCount() { return static_cast<int>(sphere.size()); }
    //void Rigister(TutorialSphere s) { sphere.push_back(s); }
    std::string GetCanBass() {  return canbassid;}
private:
  

    //std::map<Tutorial_MapName, unique_ptr<Tutorial_Flag>>tutorial_map
    //{
    //    {Tutorial_MapName::Instruction_Camera,    make_unique<Tutorial_Flag>()},
    //    {Tutorial_MapName::Instruction_Extraction,make_unique<Tutorial_Flag>()},
    //    {Tutorial_MapName::Instruction_Injection, make_unique<Tutorial_Flag>()},
    //    {Tutorial_MapName::Instruction_Jump,      make_unique<Tutorial_Flag>()},
    //    {Tutorial_MapName::Instruction_Move,      make_unique<Tutorial_Flag>()},
    //    {Tutorial_MapName::Instruction_Push,      make_unique<Tutorial_Flag>()},
    //};


    std::vector<std::unique_ptr<TutorialSphere>> sphere;
    
   // bool TutorialSphereVsPlayer(TutorialSphere& sphere_,TutorialSphere&out);
    bool TutorialSphereVsPlayer(TutorialSphere& sphere_);
public:
    void Add_TutorialSphere(std::unique_ptr<TutorialSphere>sphere_) { sphere.push_back(move(sphere_)); };
private:
    Tutorial_MapName oldSphereType=Tutorial::Tutorial_MapName::Instruction_Camera;
    Tutorial_MapName nowspheretype = Tutorial_MapName::Instruction_Camera;
    DirectX::XMFLOAT2 start;
    DirectX::XMFLOAT2 end;
    bool isInstruct = false;
    std::string canbassid="";
    std::vector<std::string> uiid;
public://—Í‹Æ
    float Timer = 0;
    float setTimer = 3.f;
    int cheackCount = 0;
};