#include"chain.h"

Lift_chain_Animatio_ndown::Lift_chain_Animatio_ndown()
{
    filenames.push_back(filename);
    MyType = Chain_Type::lift_P_Animatio_ndown;
    Graphics* gr = &Graphics::Instance();
   comp_->AddComponent<RenderComp>(filename,gr,filenames);
}

Lift_chain_Animatio_ndown::Lift_chain_Animatio_ndown(const char* filename_)
{
    filenames.push_back(filename_);
    MyType = Chain_Type::lift_P_Animatio_ndown;
    Graphics* gr = &Graphics::Instance();
    comp_->AddComponent<RenderComp>(filename, gr, filenames);
}

void Lift_chain_Animatio_ndown::ResultInfo()
{
    BaseChain::ResultInfo();
    OutputDebugStringA("\n");
    OutputDebugStringA("animation_speed: "); OutputDebugStringA(to_string( comp_->GetComponent<RenderComp>()->anim.animSpeed).c_str()); OutputDebugStringA(";"); OutputDebugStringA("\n");
}

void Lift_chain_Animatio_ndown::Update(float elapsedTime)
{
    comp_->GetComponent<RenderComp>()->anim.animSpeed = elapsedTime;
    comp_->GetComponent<RenderComp>()->anim.stop_animation=true;
    Gimic* gimic = LiftCheck();
    if (gimic)
    {
        comp_->GetComponent<RenderComp>()->anim.stop_animation = false;
        if (gimic->GetIsLift() == ObjType::heavy || gimic->GetIsLift() == ObjType::Super_heavy)
        {
             comp_->GetComponent<RenderComp>()->anim.animationEnd = false;
            if ( comp_->GetComponent<RenderComp>()->anim.animationStart)
            {
                 comp_->GetComponent<RenderComp>()->anim.stop_animation = true;
                 comp_->GetComponent<RenderComp>()->Color.w = 0.0f;
            }
        }
        else if (gimic->GetIsLift() == ObjType::cution || gimic->GetIsLift() == ObjType::Super_cution)
        {
             comp_->GetComponent<RenderComp>()->anim.animationStart = false;
             comp_->GetComponent<RenderComp>()->Color.w = 1;
            
        }
    }
   
  
    animation::keyframe* kefreame_ = 
         comp_->GetComponent<RenderComp>()->anim.getKeyFreame( comp_->GetComponent<RenderComp>()->anim.animSpeed, comp_->GetComponent<RenderComp>()->anim.LoopAnimation);
     comp_->GetComponent<RenderComp>()->GetModel()->kefreame = kefreame_;
     comp_->GetComponent<RenderComp>()->anim.update_animation(* comp_->GetComponent<RenderComp>()->GetModel()->kefreame);
     comp_->GetComponent<TransformComp>()->UpdateTransform();
}

void Lift_chain_Animatio_ndown::Gui()
{

    using namespace ImGui;
    BaseGui();
     comp_->GetComponent<RenderComp>()->RenderCompGui();
    Text(ID.c_str());
}
