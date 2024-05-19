#include"Gimic.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include"Graphics/DebugRenderer.h"
#include"variable_management_class_for_hit_test.h"
#include <Graphics/graphics.h>
using namespace DirectX;

Lift::Lift(ID3D11Device* device, XMFLOAT3 pos_)
{
    model = make_unique<Model>(device,filename,true,0.0f);
    Baria_Wall = make_unique<Model>(device, box_filename, true, 0.0f);
    Position = pos_;
    BoxPosition = Position;
    StartPos = pos_;
    BoxScale = { 55.655f,1.f,52.694f };
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    
    Gimic_type = Gimic_Type::Lift;
    type_ = Gimic::LiftType::LEFT;
    SetisLift(ObjType::heavy);
}
Lift::Lift(ID3D11Device* device, XMFLOAT3 pos_, const char* filename_)
{
    model = make_unique<Model>(device, filename_, true, 0.0f);
    Baria_Wall = make_unique<Model>(device, box_filename, true, 0.0f);
    Position = pos_;
    BoxPosition = Position;
    StartPos = pos_;
    BoxScale = { 55.655f,1.f,52.694f };
    initialaize_Set_attribute(ObjType::null, ObjType::null);

    Gimic_type = Gimic_Type::Lift;
    type_ = Gimic::LiftType::LEFT;
    SetisLift(ObjType::heavy);
}
void Lift::Update(float elapsedTime)
{
    oldWorldTransform = BoxTransform;
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat(&StartPos.y);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat(&EndPos.y);
    DirectX::XMVECTOR Position_ = DirectX::XMLoadFloat(&Position.y);
    moveRate = moveSpeed * elapsedTime;
#if 1
    if (isLift_Objtype==ObjType::heavy|| isLift_Objtype == ObjType::Super_heavy)
    {
        DirectX::XMVECTOR P = DirectX::XMVectorLerp(Position_, Goal, moveRate);
        DirectX::XMStoreFloat(&Position.y, P);
        if (Position.y <= EndPos.y)
        {
            DirectX::XMStoreFloat(&Position.y,Goal);
        }
    }
    else  if (isLift_Objtype == ObjType::cution || isLift_Objtype == ObjType::Super_cution)
    {
        
        DirectX::XMVECTOR P = DirectX::XMVectorLerp(Position_, Start, moveRate);
        DirectX::XMStoreFloat(&Position.y,P);
        if (Position.y >= StartPos.y)
        {
            DirectX::XMStoreFloat(&Position.y, Start);
        }
    }
    
#else
    if (isLift_Objtype ==  || isLift_Objtype == ObjType::Super_heavy)
    {
        DirectX::XMVECTOR P = DirectX::XMVectorLerp(Position_, Goal, moveRate);
        DirectX::XMStoreFloat(&Position.y, P);
        if (Position.y <= EndPos.y)
        {
            DirectX::XMStoreFloat(&Position.y, Goal);
        }
    }
    else  if (isLift_Objtype == ObjType::cution || isLift_Objtype == ObjType::Super_cution)
    {

        DirectX::XMVECTOR P = DirectX::XMVectorLerp(Position_, Start, moveRate);
        DirectX::XMStoreFloat(&Position.y, P);
        if (Position.y >= StartPos.y)
        {
            DirectX::XMStoreFloat(&Position.y, Start);
        }
    }
#endif
    BoxPosition = Position;
    BoxPosition.y -= 0.4f;
    UpdateTransform();
    IsWall_UpdateTransform();
    meshes = Baria_Wall->meshes;
    IdentityMatrix();//単位行列に変換する用の関数
    
}

void Lift::Render(RenderContext* rc)
{
   
    model->render(Graphics::Instance().GetDeviceContext(), Transform);
    Baria_Wall->render(Graphics::Instance().GetDeviceContext(), BoxTransform);
}

void Lift::Gui()
{
    using namespace ImGui;

    BaseGui();
    if (ImGui::CollapsingHeader("Wall_Paramerter", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::CollapsingHeader("Wall_Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("Wall_position"))
            {

                XMFLOAT3 pos{ BoxPosition };
                ImGui::InputFloat("Wall_Position.x:", &pos.x);
                ImGui::InputFloat("Wall_Position.y:", &pos.y);
                ImGui::InputFloat("Wall_Position.z:", &pos.z);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Wall_scale"))
            {
                XMFLOAT3 scale{ BoxScale };
                ImGui::InputFloat("Wall_scale.x:", &scale.x);
                ImGui::InputFloat("Wall_scale.y:", &scale.y);
                ImGui::InputFloat("Wall_scale.z:", &scale.z);

                ImGui::TreePop();
            }

        }

        if (ImGui::CollapsingHeader("move_Wall_obj", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNode("move_Wall_position"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 pos{};
                ImGui::SliderFloat("move_Wall_Position.x:", &pos.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_Wall_Position.y:", &pos.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_Wall_Position.z:", &pos.z, valu.Min.z, valu.Max.z);
                BoxPosition.x += pos.x;
                BoxPosition.y += pos.y;
                BoxPosition.z += pos.z;
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("move_Wall_scale"))
            {
                Gui_parameter_Valu valu = parameter_valu;
                XMFLOAT3 scale{};
                ImGui::SliderFloat("move_Wall_scale.x:", &scale.x, valu.Min.x, valu.Max.x);
                ImGui::SliderFloat("move_Wall_scale.y:", &scale.y, valu.Min.y, valu.Max.y);
                ImGui::SliderFloat("move_Wall_scale.z:", &scale.z, valu.Min.z, valu.Max.z);
                BoxScale.x += scale.x;
                BoxScale.y += scale.y;
                BoxScale.z += scale.z;
                ImGui::TreePop();
            }

        }
    }
    if (CollapsingHeader("Gimic_MovePoint"))
    {
        Gui_parameter_Valu val;
        if (TreeNode("StartPoint"))
        {
            InputFloat("StartPos.x", &StartPos.x);
            InputFloat("StartPos.y", &StartPos.y);
            InputFloat("StartPos.z", &StartPos.z);
            XMFLOAT3 pos2{};
            InputFloat("change_StartPos.x", &pos2.x, val.Min.x, val.Max.x);
            InputFloat("change_StartPos.y", &pos2.y, val.Min.y, val.Max.y);
            InputFloat("change_StartPos.z", &pos2.z, val.Min.z, val.Max.z);
            StartPos.x+=pos2.x;
            StartPos.y+=pos2.y;
            StartPos.z+=pos2.z;
            TreePop();
        }
        if (TreeNode("EndPoint"))
        {

            InputFloat("EndPos.x", &EndPos.x);
            InputFloat("EndPos.y", &EndPos.y);
            InputFloat("EndPos.z", &EndPos.z);
            XMFLOAT3 pos1{};
            SliderFloat("change_EndPos.x", &pos1.x, val.Min.x, val.Max.x);
            SliderFloat("change_EndPos.y", &pos1.y, val.Min.y, val.Max.y);
            SliderFloat("change_EndPos.z", &pos1.z, val.Min.z, val.Max.z);
            EndPos.x+=pos1.x;
            EndPos.y+=pos1.y;
            EndPos.z+=pos1.z;
            TreePop();
        }
        Gui_parameter_Valu param{};
        float sp = 0;
        InputFloat("moveSpeed", &moveSpeed);
        SliderFloat("Change_moveSpeed", &sp, param.Min.x, param.Max.x);
        moveSpeed += sp;
    }
    if (CollapsingHeader("TypeParam"))
    {
        if (TreeNode("change_LiftType"))
        {
            if (Button("Type_Left"))this->SetLiftType(LiftType::LEFT);
            if (Button("Type_Right"))this->SetLiftType(LiftType::RIGHT);
            TreePop();
        }
        auto type = [](LiftType t)
            {
                switch (t)
                {
                case Gimic::LiftType::LEFT:
                    return "Left";
                    break;
                case Gimic::LiftType::RIGHT:
                    return "Right";
                    break;
                default:
                    break;
                }
            };
        Text("type:", type(type_));
    }
}

bool Lift::Raycast(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, HitResult& Hit)
{
   
    VMCFHT& ince = VMCFHT::instance();
    // 前回のワールド行列と逆行列を求める
    DirectX::XMMATRIX WorldTransForm = DirectX::XMLoadFloat4x4(&oldWorldTransform);
    DirectX::XMMATRIX inverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransForm);

    //前回のローカル空間でのレイに変更する
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);

    DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, inverseWorldTransform);
    DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, inverseWorldTransform);

    DirectX::XMFLOAT3 loacalStart, loacalEnd;
    DirectX::XMStoreFloat3(&loacalStart, LocalStart);
    DirectX::XMStoreFloat3(&loacalEnd, LocalEnd);
    HitResult hit;
    if (ince.moveStage_raycast(loacalStart, loacalEnd, Hit,meshes))
    {

        //前回のローカル空間から今回のワールド空間へ変換
        //前回から今回にかけて変更された内容が乗っているオブジェクトに反映される
        WorldTransForm = DirectX::XMLoadFloat4x4(&BoxTransform);
        DirectX::XMVECTOR LocalPosition = DirectX::XMLoadFloat3(&hit.position);
        DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(LocalPosition, WorldTransForm);

        DirectX::XMVECTOR LocalNormal = DirectX::XMLoadFloat3(&hit.normal);
        DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(LocalNormal, WorldTransForm);

        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
        DirectX::XMVECTOR Dist = DirectX::XMVector3Length(Vec);
        DirectX::XMStoreFloat3(&Hit.position, WorldPosition);
        DirectX::XMStoreFloat3(&Hit.normal, WorldNormal);
        DirectX::XMStoreFloat(&Hit.distance, Dist);
        Hit.materialIndex = hit.materialIndex;
        return true;
    }
    return false;
 
}

void Lift::ResultInfo()
{
    auto lifttypeList = [](Gimic::LiftType type)
        {
            switch (type)
            {
            case Gimic::LiftType::LEFT:
                return "LEFT";
                break;
            case Gimic::LiftType::RIGHT:
                return "RIGHT";
                break;
            }

        };
    OutputDebugStringA(lifttypeList(GetLiftType()));

    OutputDebugStringA("StartPos.x: "); OutputDebugStringA(to_string(StartPos.x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("StartPos.y: "); OutputDebugStringA(to_string(StartPos.y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("StartPos.z: "); OutputDebugStringA(to_string(StartPos.z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("EndPos.x: "); OutputDebugStringA(to_string(EndPos.x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("EndPos.y: "); OutputDebugStringA(to_string(EndPos.y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("EndPos.z: "); OutputDebugStringA(to_string(EndPos.z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("\n");
    OutputDebugStringA("Position.x: "); OutputDebugStringA(to_string(Position.x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.y: "); OutputDebugStringA(to_string(Position.y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.z: "); OutputDebugStringA(to_string(Position.z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("\n");
    OutputDebugStringA("Scale.x: "); OutputDebugStringA(to_string(Scale.x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.y: "); OutputDebugStringA(to_string(Scale.y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.z: "); OutputDebugStringA(to_string(Scale.z).c_str()); OutputDebugStringA("\n");


    OutputDebugStringA("\n");
}

void Lift::IdentityMatrix()
{
    for (auto& mesh : meshes)
    {
        mesh.default_global_transform = {
            1, 0, 0, 0, 
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1 };
    }
}
