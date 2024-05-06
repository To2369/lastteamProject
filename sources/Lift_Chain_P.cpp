#include"chain.h"
#include"objectManajer.h"
Lift_chain_P::Lift_chain_P()
{
    transform = make_unique<TransformComp>();
    render = make_unique<RenderComp>(filename,Graphics::Instance());
    MyType = Chain_Type::lift_P_Not_Animation;
    render->Color = {0,0,0,0};
}

void Lift_chain_P::Update(float elapsedTime)
{
    Gimic* gimic = LiftCheck();
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
                    if (obj->GetRenderComp()->anim.animationEnd)
                    {
                        render->Color.w = 1;
                    }
                }
            }
            
        }
        else if (gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {

            render->Color.w = 0;
           
        }
    }
    transform->UpdateTransform();
}

void Lift_chain_P::Gui()
{

    using namespace ImGui;
    BaseGui();
    render->RenderCompGui();
    Text(ID.c_str());
}