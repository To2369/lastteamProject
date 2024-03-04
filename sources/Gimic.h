#pragma once
#include"object.h"
#include"Graphics/RenderContext.h"
enum class Gimic_Type
{
    Switch,
    Door,
    Goal,
    null
};

class Gimic :public Object
{
public:
    Gimic() {};
    virtual ~Gimic() {};

    virtual void Update(float elapsedTime) {};
    virtual void Render(RenderContext* rc) {};
   
   
public:
    void SetBootFlag(bool f) { bootFlag.push_back(f);}
    void SetSwitchFlag(bool f) { switchFlag = f; }
public:
    ObjType GetMyObjeFlagType()const { return MyObjeFlagType; }
    Gimic_Type Get_GimicType()const { return Gimic_type; }//自分のタイプを取得
   
    //ObjType Get_GameObj() { return get_gameobj; }//今自分に乗ってるオブジェクト取得
    bool Get_SwitchFlag()const { return switchFlag; }
public://gimicに対して動作するgimic
    Gimic_Type GetMyGimicFlagType()const { return MyGimicFlagType; }
    bool GetBootFlag()const { return bootFlag[0]; }
    bool GetBootFlag(int i)const { return bootFlag[i]; }


protected:
    bool gimic_VS_Object();
    vector<bool> bootFlag;
    bool switchFlag=false;
    void Gimic_effect(Gimic_Type type);
    void Gimic_VS_GimicFlagBoot();
    Gimic_Type Gimic_type=Gimic_Type::null;
    //ObjType get_gameobj;
    ObjType MyObjeFlagType=ObjType::null;//自分がどのオブジェクトタイプでギミックが起動するかの変数
    Gimic_Type MyGimicFlagType = Gimic_Type::null;//自分がどのGimicタイプでギミックが起動するかの変数

};

class Switch :public Gimic
{
public:
    Switch(ID3D11Device* device);
    ~Switch()override {
      
    };
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;


public:
  

private:
    const char* filename = ".\\resources\\ground.fbx";
};


class Door :public Gimic
{
public:
    Door(ID3D11Device* device);
    ~Door()override {};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;


public:


private:
    const char* filename = ".\\resources\\ground.fbx";
};

class Goal :public Gimic
{
public:
    Goal(ID3D11Device* device);
    ~Goal()override {}
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    bool GoalInPosition();
public:
    bool Get_GoalFlag()const { return Goal_Flag; }
 
private:
    float radius = 0.2f;
    bool Goal_Flag = false;
    const char*filename= ".\\resources\\ground.fbx";
};