#include"Gimic.h"
#include"Graphics/graphics.h"

Move_Wall::Move_Wall(ID3D11Device* device)
{
}

Move_Wall::~Move_Wall()
{
}

void Move_Wall::Update(float elapsedTime)
{
}

void Move_Wall::Render(RenderContext* rc)
{
    Graphics& ince_gr = Graphics::Instance();
    model->render(ince_gr.GetDeviceContext(), Transform);
}
