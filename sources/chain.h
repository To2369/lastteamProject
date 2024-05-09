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
class BaseChain :public Comp
{
public:
    BaseChain() {};
    virtual ~BaseChain() override{}
    bool GetDestroy() { return destroy_this; };
public:
    void SetID(std::string id) { ID = id; }
    std::string GetID() { return ID; }
    Chain_Type GetMyType() { return MyType; }
   virtual void ResultInfo();
    Gimic* LiftCheck();
protected:
    Chain_Type MyType = Chain_Type::null;
    std::string ID;
    bool destroy_this = false;
};
class Lift_chain_s:public BaseChain//チェーンが短いやつ
{
public:
    Lift_chain_s();
    ~Lift_chain_s()override{};
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
    ~Lift_chain_l()override {};
    void Update(float elapsedTime)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage3\\C_L.fbx";
   
};


class Lift_chain_P :public BaseChain//リフトの上にペンギンが乗ってる、アニメーションなし
{
public:
    Lift_chain_P();
    ~Lift_chain_P()override {};
    void Update(float elapsedTime)override;
    //void BaseChainGUI();
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage3\\rihuto_P.fbx";
   // const char* filename = ".\\resources\\stage3\\C_L2.fbx";

};

class Lift_chain_Animatio_ndown :public BaseChain//リフトの上にペンギンが乗ってる、アニメーションあり
{
public:
    Lift_chain_Animatio_ndown();
    ~Lift_chain_Animatio_ndown()override { filenames.clear(); };
    void ResultInfo()override;
    void Update(float elapsedTime)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage3\\rihuto_Pdown_P.fbx";
  //    const char* filename = ".\\resources\\stage3\\R_P.fbx";
    std::vector<std::string>filenames;
};