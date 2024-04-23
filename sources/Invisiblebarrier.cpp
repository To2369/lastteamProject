#include "InvisibleBarria.h"
using namespace std;
using namespace DirectX;
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif

InvisibleBarrier::InvisibleBarrier(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    type = Static_ObjType::Invisible_Wall;

    Scale = { 291.298f,198.939f,20.385 };
}

void InvisibleBarrier::Update(float elapsedTime)
{

    UpdateTransform();
}

void InvisibleBarrier::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0, { 0.5f,0.5f,0.5f,0.4f });

}

void InvisibleBarrier::Gui()
{
    BaseGui();

}
