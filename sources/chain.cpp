#include "chain.h"
#include"objectManajer.h"
Gimic* BaseChain::LiftCheck()
{
    Objectmanajer& ince_o = Objectmanajer::incetance();
    int count = ince_o.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        
        Gimic* obj = ince_o.Get_GameGimic(i);
        if(obj->Get_GimicType() != Gimic_Type::Lift)continue;
        if (obj->GetGimicID() == this->GetID())
        {
            if (obj->GetIsLift() == ObjType::heavy || obj->GetIsLift() == ObjType::Super_heavy)
            {
                this-> comp_->GetComponent<RenderComp>()->anim.animSpeed = -this-> comp_->GetComponent<RenderComp>()->anim.animSpeed;
               
            }
            else if (obj->GetIsLift() == ObjType::cution || obj->GetIsLift() == ObjType::Super_cution)
            {
                this-> comp_->GetComponent<RenderComp>()->anim.animSpeed = +this-> comp_->GetComponent<RenderComp>()->anim.animSpeed;
                
            }
            return obj;
        }
    }
    return nullptr;
}

void BaseChain::BaseGui()
{
    if (ImGui::CollapsingHeader("Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("position"))
            {

                XMFLOAT3 pos{  comp_->GetComponent<TransformComp>()->GetPosition() };
                ImGui::InputFloat("Position.x:", &pos.x);
                ImGui::InputFloat("Position.y:", &pos.y);
                ImGui::InputFloat("Position.z:", &pos.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("scale"))
            {
                XMFLOAT3 scale{  comp_->GetComponent<TransformComp>()->Getscale() };
                ImGui::InputFloat("scale.x:", &scale.x);
                ImGui::InputFloat("scale.y:", &scale.y);
                ImGui::InputFloat("scale.z:", &scale.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("angle"))
            {
                XMFLOAT3 angle{  comp_->GetComponent<TransformComp>()->GetAngle() };
                ImGui::InputFloat("angle.x:", &angle.x);
                ImGui::InputFloat("angle.y:", &angle.y);
                ImGui::InputFloat("angle.z:", &angle.z);

                ImGui::TreePop();
            }
        }

        if (ImGui::CollapsingHeader("move_obj", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("move_position"))
            {
                Gui_parameter_Valu valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("move_Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                 comp_->GetComponent<TransformComp>()->AddPosition(pos);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_scale"))
            {
                Gui_parameter_Valu valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("move_scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                 comp_->GetComponent<TransformComp>()->AddScale(scale);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Angle"))
            {
                Gui_parameter_Valu valu;
                XMFLOAT3 Angle{};
                ImGui::SliderFloat("move_angle.x:", &Angle.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_angle.y:", &Angle.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_angle.z:", &Angle.z, valu.Min.z, valu.Max.z);
                 comp_->GetComponent<TransformComp>()->AddAngle(Angle);
                ImGui::TreePop();
            }
        }
    }

}


void BaseChain::ResultInfo()
{
    auto type_map = [](Chain_Type type)
        {
            switch (type)
            {
            case Chain_Type::lift_chain_S:
                return "lift_chain_S";

                break;
            case Chain_Type::lift_chain_L:

                return "lift_chain_L";
                break;
            case Chain_Type::lift_P_Animatio_ndown:

                return "lift_P_Animatio_ndown";
                break;
            case Chain_Type::lift_P_Not_Animation:

                return "lift_P_Not_Animation";
                break;
            case Chain_Type::null:

                return "null";
                break;
           
            }
            return "";
        };

    OutputDebugStringA(type_map(this->MyType));
    OutputDebugStringA("\n");
    OutputDebugStringA("ID:"); OutputDebugStringA(ID.c_str());
    OutputDebugStringA("\n");
    OutputDebugStringA("Position.x: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->GetPosition().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.y: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->GetPosition().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.z: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->GetPosition().z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("\n");
    OutputDebugStringA("Scale.x: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->Getscale().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.y: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->Getscale().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.z: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->Getscale().z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("\n");
    OutputDebugStringA("Angle.x: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->GetAngle().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Angle.y: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->GetAngle().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Angle.z: "); OutputDebugStringA(to_string( comp_->GetComponent<TransformComp>()->GetAngle().z).c_str()); OutputDebugStringA("\n");


    OutputDebugStringA("\n");
}