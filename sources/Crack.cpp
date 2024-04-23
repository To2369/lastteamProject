#include"Crack.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include"objectManajer.h"
#endif
Crack::Crack(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_heavy, ObjType::Crack);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;

    UpdateTransform();
}

void Crack::Update(float elapsedTime)
{
    color = InitColor();
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();
    if (!Get_isGimic_UpPosNow())RayCastGround();
    ObjType_effect(elapsedTime);
    UpdateTransform();

}

void Crack::Render(RenderContext* rc)
{


    model->render(rc->deviceContext, Transform, 0.f, color);
}

void Crack::Gui()
{
    BaseGui();


}
