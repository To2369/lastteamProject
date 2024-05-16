#include"Stage.h"


Stage_1_3::Stage_1_3(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true, 0.0f);

}

Stage_1_3::~Stage_1_3()
{
}

void Stage_1_3::Update(float elapsedTime)
{
    UpdateTransform();
}

void Stage_1_3::Render(RenderContext* rc)
{
    model->render(rc->deviceContext,Transform,0.0f);
}

