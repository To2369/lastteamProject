#include"chain.h"

Lift_chain_Animatio_ndown::Lift_chain_Animatio_ndown()
{
    filenames.push_back(filename);
    transform = make_unique<TransformComp>();
    render = make_unique<RenderComp>(filename, Graphics::Instance(), filenames);
    render->anim.LoopAnimation = false;
    MyType = Chain_Type::lift_P_Animatio_ndown;
}

Lift_chain_Animatio_ndown::Lift_chain_Animatio_ndown(const char* filename_)
{
    filenames.push_back(filename_);
    transform = make_unique<TransformComp>();
    render = make_unique<RenderComp>(filename_, Graphics::Instance(), filenames);
    render->anim.LoopAnimation = false;
    MyType = Chain_Type::lift_P_Animatio_ndown;
}

void Lift_chain_Animatio_ndown::ResultInfo()
{
    BaseChain::ResultInfo();
    OutputDebugStringA("\n");
    OutputDebugStringA("animation_speed: "); OutputDebugStringA(to_string(render->anim.animSpeed).c_str()); OutputDebugStringA(";"); OutputDebugStringA("\n");
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
            render->anim.animationEnd = false;
            if (render->anim.animationStart)
            {
                render->anim.stop_animation = true;
                render->Color.w = 0.0f;
            }
        }
        else if (gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {
            render->anim.animationStart = false;
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
