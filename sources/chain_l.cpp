#include"chain.h"

Lift_chain_l::Lift_chain_l()
{
    
    transform = make_unique<TransformComp>();
    //render = make_unique<RenderComp>(filename, Graphics::Instance(),filenames);
    render = make_unique<RenderComp>(filename, Graphics::Instance());
    MyType = Chain_Type::lift_chain_L;
}

void Lift_chain_l::Update(float elapsedTime)
{
    Gimic* gimic = LiftCheck();
    if (gimic)
    {
        if (gimic->GetIsLift() == ObjType::heavy || gimic->GetIsLift() == ObjType::Super_heavy)
        {
            render->Color.w = 1;
        }
        else if (gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {
            render->Color.w = 0;
        }
    }

    transform->UpdateTransform();
}

void Lift_chain_l::Gui()
{
    using namespace ImGui;
    BaseGui();
    render->RenderCompGui();
    Text(ID.c_str());
}
