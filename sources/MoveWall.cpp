#include"Gimic.h"
#include"Graphics/graphics.h"
using namespace std;
using namespace DirectX;
Move_Wall::Move_Wall(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
}

Move_Wall::Move_Wall(ID3D11Device* device, const char* filename_)
{
    model = make_unique<Model>(device,filename_,true);

}

Move_Wall::~Move_Wall()
{
}

void Move_Wall::Update(float elapsedTime)
{
    UpdateTransform();
}

void Move_Wall::Render(RenderContext* rc)
{
    Graphics& ince_gr = Graphics::Instance();
    model->render(ince_gr.GetDeviceContext(), Transform);
}
