#pragma once
#include"Comp.h"
#include"Gimic.h"
enum class Chain_Type
{
    lift_chain_S,
    lift_chain_L,
    lift_P_Animatio_ndown,
    lift_P_Not_Animation,
    null
};
class BaseChain
{
public:
    virtual ~BaseChain() {
    
      
    }
    BaseChain() {
        comp_ = new object();
        
        
       /* comp_=*/ comp_->AddComponent<TransformComp>();
      // comp_->AddComponent<RenderComp>();
        
    };
    virtual void Update(float elapsedTime)=0;
    bool GetDestroy() { return destroy_this; };
public:
    void SetID(std::string id) { ID = id; }
    std::string GetID() { return ID; }
    Chain_Type GetMyType() { return MyType; }
   virtual void ResultInfo();
    Gimic* LiftCheck();
    virtual void Gui()=0;
    void BaseGui();
    object* GetComp() { return comp_; };
protected:
   
    object*comp_;
    Chain_Type MyType = Chain_Type::null;
    std::string ID;
    bool destroy_this = false;
};
class Lift_chain_s:public BaseChain//チェーンが短いやつ
{
public:
    Lift_chain_s();
    Lift_chain_s(const char*filename_);
    ~Lift_chain_s()override {
        if (comp_)
        {
            
            delete comp_;
            comp_ = nullptr;
        }
       
    };
public:
    void Update(float elapsedTme)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage3\\C_S.fbx";
};


class Lift_chain_l:public BaseChain//チェーンが長いやつ
{
public:
    Lift_chain_l();
    Lift_chain_l(const char* filename_,float offset_=0);
    ~Lift_chain_l() override{
       
        if (comp_)
        {
            
            delete comp_;
            comp_ = nullptr;
        }
       
       
    };
    void Update(float elapsedTime)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage3\\C_L.fbx";
    float offset = 0;
};


class Lift_chain_P :public BaseChain//リフトの上にペンギンが乗ってる、アニメーションなし
{
public:
    Lift_chain_P();
    Lift_chain_P(const char*filename_);
    ~Lift_chain_P()override {
        if (comp_)
        {
            delete comp_;
            comp_ = nullptr;
        }
       
    };
    void Update(float elapsedTime)override;
    //void BaseChainGUI();
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage3\\rihutoP.fbx";
   // const char* filename = ".\\resources\\stage3\\C_L2.fbx";

};

class Lift_chain_Animatio_ndown :public BaseChain//リフトの上にペンギンが乗ってる、アニメーションあり
{
public:
    Lift_chain_Animatio_ndown();
    Lift_chain_Animatio_ndown(const char*filename_);
    ~Lift_chain_Animatio_ndown()override {
        if (comp_)
        {
           
            delete comp_;
            comp_ = nullptr;
        }
       
        filenames.clear(); };
    void ResultInfo()override;
    void Update(float elapsedTime)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage3\\rihuto_Pdown_P.fbx";
  //    const char* filename = ".\\resources\\stage3\\R_P.fbx";
    std::vector<std::string>filenames;
};