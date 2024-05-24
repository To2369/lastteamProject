#include"chain.h"
#include"objectManajer.h"
Lift_chain_P::Lift_chain_P()
{
    Graphics* gr = &Graphics::Instance();
    comp_ ->AddComponent<RenderComp>(filename, gr);
    MyType = Chain_Type::lift_P_Not_Animation;
     comp_->GetComponent<RenderComp>()->Color = {1,1,1,1};
}

Lift_chain_P::Lift_chain_P(const char* filename_)
{
    Graphics* gr = &Graphics::Instance();
    comp_ ->AddComponent<RenderComp>(filename, gr);
    MyType = Chain_Type::lift_P_Not_Animation;
     comp_->GetComponent<RenderComp>()->Color = { 1,1,1,1 };
}

void Lift_chain_P::Update(float elapsedTime)
{
    Gimic* gimic = LiftCheck();
     comp_->GetComponent<RenderComp>()->Color.w = 1;
    if (gimic)
    {
        if (gimic->GetIsLift() == ObjType::heavy || gimic->GetIsLift() == ObjType::Super_heavy)
        {
            Objectmanajer& ince_o = Objectmanajer::incetance();
            int count = ince_o.Get_GameLiftChainCount();
            for (int i = 0; i < count; i++)
            {
                BaseChain* obj = ince_o.Get_GameLiftChain(i);
                if (obj->GetMyType() == Chain_Type::lift_P_Animatio_ndown && obj->GetID() == this->GetID())
                {
                    if (obj->GetComp()->GetComponent<RenderComp>()->anim.animationStart)
                    {
                         comp_->GetComponent<RenderComp>()->Color.w = 1;
                    }
                }
            }
            
        }
        else if (gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {

             comp_->GetComponent<RenderComp>()->Color.w = 0;
           
        }
    }
     comp_->GetComponent<TransformComp>()->UpdateTransform();
}

void Lift_chain_P::Gui()
{

    using namespace ImGui;
    BaseGui();
     comp_->GetComponent<RenderComp>()->RenderCompGui();
    Text(ID.c_str());
}
