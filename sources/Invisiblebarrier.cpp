#include "InvisibleBarria.h"
using namespace std;
using namespace DirectX;
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include "Graphics/graphics.h"

InvisibleBarrier::InvisibleBarrier(ID3D11Device* device,XMFLOAT3 Scale_)
{
    model = make_unique<Model>(device, filename, true);
    type = Static_ObjType::Invisible_Wall;
    Scale = Scale_;
    //Scale = { 291.298f,198.939f,20.385 };
}

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
    Graphics& graphics = Graphics::Instance();
    graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(1), nullptr, 0xFFFFFFFF);
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0, {0.5f,0.5f,0.5f,0.1f});

    graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);
}

void InvisibleBarrier::Gui()
{
    BaseGui();

}
