#include"Stage.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include "Graphics/graphics.h"
#include"Tutorial.h"
#include"UI.h"
using namespace DirectX;
using namespace std;


Stage_1_1::Stage_1_1(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    Scale = { 1.f,1.f,1.f };
    child_model = make_unique<Model>(device,filename2,true);
  
}

void Stage_1_1::Update(float elapsedTime)
{
   
   // Position.y = +20;
    Position.y = -2.f;
    child_Position.x = 0.1f;
    child_Position.y = -2.6f;
    UpdateTransform();
    Child_UpdateTransform();
   
}

void Stage_1_1::Render(RenderContext* rc)
{
    model->render(Graphics::Instance().GetDeviceContext(), Transform);
    child_model->render(Graphics::Instance().GetDeviceContext(),child_transform);
}

void Stage_1_1::Child_UpdateTransform()
{
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
     { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
     { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
     { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
     { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    const float scale_factor = 0.01f;
    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };

    //スケール行列を作成
    XMMATRIX S = XMMatrixScaling(child_Scale.x, child_Scale.y, child_Scale.z);
    //回転行列作成
    /*XMMATRIX X = XMMatrixRotationX(Angle.x);
    XMMATRIX Y = XMMatrixRotationY(Angle.y);
    XMMATRIX Z = XMMatrixRotationZ(Angle.z);*/
    XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(child_Angle.x, child_Angle.y, child_Angle.z);
    //位置行列を作成
    XMMATRIX T = XMMatrixTranslation(child_Position.x, child_Position.y, child_Position.z);
    //4つの行列を組み合わせて、ワールド座標を作成
    DirectX::XMMATRIX W = C * S * R * T;
    //計算したワールド座標を取り出す
    DirectX::XMStoreFloat4x4(&child_transform, W);
}

stage_Yuka::stage_Yuka(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    Scale = { 5.988f,3.826f,225.357f };
    Position.x = 0.606f;
    Position.y = 22.432f;
    Position.z = -0.519f;
}

void stage_Yuka::Update(float elapsedTime)
{

    UpdateTransform();
}

void stage_Yuka::Render(RenderContext* rc)
{
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0, {1,1,1,0.2});
}

void stage_Yuka::Gui()
{
    BaseGui();
}


/////////////////////////////////////////////////////
stage_OBJ::stage_OBJ(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    type = Static_ObjType::Static_Object;
    Scale = { 1.f,1.f,1.f };
    Position.y = 24;
}
stage_OBJ::stage_OBJ(ID3D11Device* device, const char* filename_)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device, filename_, true);
    type = Static_ObjType::Static_Object;
    Scale = { 1.f,1.f,1.f };
    Position.y = 24;
}
void stage_OBJ::Update(float elapsedTime)
{
    Position.x = 0.708f;
    Position.y = 0.920f;
    Position.z = -1.f;


    UpdateTransform();
}

void stage_OBJ::Render(RenderContext* rc)
{
    model->render(Graphics::Instance().GetDeviceContext(), Transform);
}

void stage_OBJ::Gui()
{
    BaseGui();
    /*  auto face_type = [](FaceBoundingBoxCollition::face face_)
          {
              switch (face_)
              {
              case FaceBoundingBoxCollition::face::Right:
                  return "Right";
                  break;
              case FaceBoundingBoxCollition::face::Left:
                  return "Left";
                  break;
              case FaceBoundingBoxCollition::face::Front:
                  return "Front";
                  break;
              case FaceBoundingBoxCollition::face::Back:
                  return "Back";
                  break;
              }
              return "null";
          };

      ImGui::Text("hitface:%s",face_type(Getface()));*/
}

