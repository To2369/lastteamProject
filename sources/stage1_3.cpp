#include "Stage.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
using namespace DirectX;
using namespace std;
Stage_1_3::Stage_1_3(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device, filename, true);
    Scale = { 1.f,1.f,1.f };
}

void Stage_1_3::Update(float elapsedTime)
{
    UpdateTransform();
}

void Stage_1_3::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform);

}