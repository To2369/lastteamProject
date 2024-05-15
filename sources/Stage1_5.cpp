#include"Stage.h"


Stage3::Stage3(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true, 0.0f);

}

Stage3::~Stage3()
{
}

void Stage3::Update(float elapsedTime)
{
    UpdateTransform();
}

void Stage3::Render(RenderContext* rc)
{
    model->render(rc->deviceContext,Transform,0.0f);
}

