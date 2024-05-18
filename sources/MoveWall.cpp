#include"Gimic.h"
#include"Graphics/graphics.h"
#include"objectManajer.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
using namespace std;
using namespace DirectX;
Move_Object::Move_Object(ID3D11Device* device, DirectX::XMFLOAT3 pos, bool flags[3])
{
    model = make_unique<Model>(device, filename, true);
    Position = pos;
   StartPos = Position;
    x_flag = flags[0];
    y_flag = flags[1];
    z_flag = flags[2];
}

Move_Object::Move_Object(ID3D11Device* device, const char* filename_, DirectX::XMFLOAT3 pos, bool flags[3])
{
    model = make_unique<Model>(device,filename_,true);
    Position = pos;
    StartPos = Position;
    x_flag = flags[0];
    y_flag = flags[1];
    z_flag = flags[2];
}

Move_Object::~Move_Object()
{
}

void Move_Object::Gui()
{
    using namespace ImGui;
    BaseGui();
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
            StartPos.x += pos2.x;
            StartPos.y += pos2.y;
            StartPos.z += pos2.z;
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
            EndPos.x += pos1.x;
            EndPos.y += pos1.y;
            EndPos.z += pos1.z;
            TreePop();
        }
        Gui_parameter_Valu param{};
        float sp = 0;
        InputFloat("moveSpeed", &moveSpeed);
        SliderFloat("Change_moveSpeed", &sp, param.Min.x, param.Max.x);
        moveSpeed += sp;
        Checkbox("x_moveflag",&x_flag);
        Checkbox("y_moveflag",&y_flag);
        Checkbox("z_moveflag",&z_flag);
        
    }
}

void Move_Object::Update(float elapsedTime)
{
    Objectmanajer& ince_o = Objectmanajer::incetance();
    int count = ince_o.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        Gimic* gimic = ince_o.Get_GameGimic(i);
        if (gimic == this)continue;
        if (gimic->Get_GimicType() != Gimic_Type::Switch)continue;
        if (gimic->GetGimicID() == ID && gimic->Get_SwitchFlag())
        {
            SetBootFlag(true);
        }
    }
    if (GetBootFlag())
    {
        if (x_flag)
        {
            DirectX::XMVECTOR P = DirectX::XMVectorLerp(XMLoadFloat(&Position.x), XMLoadFloat(&EndPos.x), moveSpeed);
            XMStoreFloat3(&Position, P);
            
        }
        else if (y_flag)
        {
            DirectX::XMVECTOR P = DirectX::XMVectorLerp(XMLoadFloat(&Position.y), XMLoadFloat(&EndPos.y), moveSpeed);
            XMStoreFloat3(&Position, P);
          
        }
        else if (z_flag)
        {
            DirectX::XMVECTOR P = DirectX::XMVectorLerp(XMLoadFloat(&Position.z), XMLoadFloat(&EndPos.z), moveSpeed);
            XMStoreFloat3(&Position, P);
         
        }
    }

    RayCastGround();
    UpdateTransform();
}

void Move_Object::Render(RenderContext* rc)
{
    Graphics& ince_gr = Graphics::Instance();
    model->render(ince_gr.GetDeviceContext(), Transform);
}
