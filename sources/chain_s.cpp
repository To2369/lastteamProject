#include "chain.h"

Lift_chain_s::Lift_chain_s()
{
    transform = make_unique<TransformComp>();
    render = make_unique<RenderComp>(filename,Graphics::Instance());
    MyType = Chain_Type::lift_chain_S;
    
}

void Lift_chain_s::Update(float elapsedTme)
{
    
    Gimic*gimic=LiftCheck();
    
    render->Color.w = 1;
    if (gimic)
    {
        if (gimic->GetIsLift() == ObjType::heavy || gimic->GetIsLift() == ObjType::Super_heavy)
        {
            render->Color.w = 0;
        }
        else if(gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {
            render->Color.w = 1;
        }
    }
    transform->UpdateTransform();
  
}

void Lift_chain_s::Gui()
{

    using namespace ImGui;
    BaseGui();
    render->RenderCompGui();
    Text(ID.c_str());
}
