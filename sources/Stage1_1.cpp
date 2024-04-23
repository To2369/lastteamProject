#include"Stage.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
using namespace DirectX;
using namespace std;

Stage_1_1::Stage_1_1(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    Scale = { 1.f,1.f,1.f };

}

void Stage_1_1::Update(float elapsedTime)
{

    Position.y = +20;

    UpdateTransform();
}

void Stage_1_1::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform);
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
    model->render(rc->deviceContext, Transform, 0, { 1,1,1,0.2 });
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
    Position.y = 22.920f;
    Position.z = -1.f;


    UpdateTransform();
}

void stage_OBJ::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform);
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

