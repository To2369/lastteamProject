#include "Comp.h"
#include <object.h>


void Comp::Render()
{
    Graphics& gr = Graphics::Instance();
    render->render(gr, transform->Gettransform());
}

void Comp::BaseGui()
{
    if (ImGui::CollapsingHeader("Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("position"))
            {

                XMFLOAT3 pos{ transform->GetPosition() };
                ImGui::InputFloat("Position.x:", &pos.x);
                ImGui::InputFloat("Position.y:", &pos.y);
                ImGui::InputFloat("Position.z:", &pos.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("scale"))
            {
                XMFLOAT3 scale{ transform->Getscale() };
                ImGui::InputFloat("scale.x:", &scale.x);
                ImGui::InputFloat("scale.y:", &scale.y);
                ImGui::InputFloat("scale.z:", &scale.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("angle"))
            {
                XMFLOAT3 angle{ transform->GetAngle() };
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
                transform->AddPosition(pos);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_scale"))
            {
                Gui_parameter_Valu valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("move_scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                transform->AddScale(scale);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Angle"))
            {
                Gui_parameter_Valu valu;
                XMFLOAT3 Angle{};
                ImGui::SliderFloat("move_angle.x:", &Angle.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_angle.y:", &Angle.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_angle.z:", &Angle.z, valu.Min.z, valu.Max.z);
                transform->AddAngle(Angle);
                ImGui::TreePop();
            }
        }
    }
    
}


void RenderComp::RenderCompGui()
{
    using namespace ImGui;
    if (CollapsingHeader("Animation"))
    {
        if (TreeNode("StopAnimation"))
        {
            if (Checkbox("Animation_Flag", &anim.stop_animation))
            {
            }
            if (Button("stop"))
            {
                anim.stop_animation = !anim.stop_animation;
            }
            if (Checkbox("Loop_Flag", &anim.LoopAnimation))
            {
            }
            if (Button("Loop"))
            {
                anim.LoopAnimation = !anim.LoopAnimation;
            }
            float sp = anim.animSpeed;
            InputFloat("AnimationSpeed", &sp);
            SliderFloat("Speed_param", &anim.animSpeed, -0.01f, 0.01f);
            TreePop();
        }
        if (TreeNode("AnimationClip"))
        {
            int size = anim.animation_clips.size();
            InputInt("ClipSize",&size);
           /* if (TreeNode("ClipData"))
            {

                TreePop();
            }*/
            TreePop();
        }
    }

}
