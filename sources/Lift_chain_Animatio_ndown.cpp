#include"chain.h"

Lift_chain_Animatio_ndown::Lift_chain_Animatio_ndown()
{
    filenames.push_back(filename);
    transform = make_unique<TransformComp>();
    render = make_unique<RenderComp>(filename, Graphics::Instance(), filenames);
    render->anim.LoopAnimation = false;
    MyType = Chain_Type::lift_P_Animatio_ndown;
}

void Lift_chain_Animatio_ndown::Update(float elapsedTime)
{
    render->anim.animSpeed = elapsedTime;
    render->anim.stop_animation=true;
    Gimic* gimic = LiftCheck();
    if (gimic)
    {
        render->anim.stop_animation = false;
        if (gimic->GetIsLift() == ObjType::heavy || gimic->GetIsLift() == ObjType::Super_heavy)
        {
            if (render->anim.animationEnd)
            {
                render->Color.w = 0;
            }
        }
        else if (gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {
            render->Color.w = 1;
            
        }
    }
   
  
    animation::keyframe* kefreame_ = 
        render->anim.getKeyFreame(render->anim.animSpeed,render->anim.LoopAnimation);
    render->GetModel()->kefreame = kefreame_;
    render->anim.update_animation(*render->GetModel()->kefreame);
    transform->UpdateTransform();
}

void Lift_chain_Animatio_ndown::Gui()
{

    using namespace ImGui;
    BaseGui();
    render->RenderCompGui();
    Text(ID.c_str());
}
