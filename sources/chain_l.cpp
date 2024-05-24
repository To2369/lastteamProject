#include"chain.h"

Lift_chain_l::Lift_chain_l()
{
    //render = make_unique<RenderComp>(filename, Graphics::Instance(),filenames);
    Graphics* gr = &Graphics::Instance();

    comp_ ->AddComponent<RenderComp>(filename, gr);
    MyType = Chain_Type::lift_chain_L;
    offset = 1.5f;
}

Lift_chain_l::Lift_chain_l(const char* filename_,float offset_)
{
    //render = make_unique<RenderComp>(filename, Graphics::Instance(),filenames);z
    Graphics* gr = &Graphics::Instance();
   
    comp_ ->AddComponent<RenderComp>(filename_, gr);
    MyType = Chain_Type::lift_chain_L;
    offset = offset_;
}

void Lift_chain_l::Update(float elapsedTime)
{
    
    Gimic* gimic = LiftCheck();
    XMFLOAT3 Position = gimic ? gimic->GetPosition() : XMFLOAT3(0.f, 0.f, 0.f);
    Position.x = comp_->GetComponent<TransformComp>()->GetPosition().x;
    Position.z = comp_->GetComponent<TransformComp>()->GetPosition().z;
    Position.y += offset;
    comp_->GetComponent<RenderComp>()->Color.w = 1;
    if (gimic)
    {
        if (gimic->GetIsLift() == ObjType::heavy || gimic->GetIsLift() == ObjType::Super_heavy)
        {
            comp_->GetComponent<RenderComp>()->Color.w = 1;
        }
        else if (gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {
            comp_->GetComponent<RenderComp>()->Color.w = 0;
        }
    }
    comp_->GetComponent<TransformComp>()->SetPosition(Position);
    comp_->GetComponent<TransformComp>()->UpdateTransform();
}

void Lift_chain_l::Gui()
{
    using namespace ImGui;

    BaseGui();
    comp_->GetComponent<RenderComp>()->RenderCompGui();
    Text(ID.c_str());
}
