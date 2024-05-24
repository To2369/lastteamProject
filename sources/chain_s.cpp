#include "chain.h"

Lift_chain_s::Lift_chain_s()
{
    MyType = Chain_Type::lift_chain_S;   
    Graphics* gr = &Graphics::Instance();

    comp_->AddComponent<RenderComp>(filename, gr);
}

Lift_chain_s::Lift_chain_s(const char* filename_)
{
    MyType = Chain_Type::lift_chain_S;
    Graphics* gr = &Graphics::Instance();

    comp_->AddComponent<RenderComp>(filename_, gr);
}

void Lift_chain_s::Update(float elapsedTme)
{
    
    Gimic*gimic=LiftCheck();
    
    comp_->GetComponent<RenderComp>()->Color.w = 1;
    if (gimic)
    {
        if (gimic->GetIsLift() == ObjType::heavy || gimic->GetIsLift() == ObjType::Super_heavy)
        {
             comp_->GetComponent<RenderComp>()->Color.w = 0;
        }
        else if(gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {
             comp_->GetComponent<RenderComp>()->Color.w = 1;
        }
    }
     comp_->GetComponent<TransformComp>()->UpdateTransform();
  
}

void Lift_chain_s::Gui()
{

    using namespace ImGui;
    BaseGui();
     comp_->GetComponent<RenderComp>()->RenderCompGui();
    Text(ID.c_str());
}
