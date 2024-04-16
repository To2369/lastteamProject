#include"Stage.h"
using namespace DirectX;
using namespace std;

Stage_1_1::Stage_1_1(ID3D11Device* device)
{
    model = make_unique<Model>(device,filename,true);
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    Scale = { 1.f,1.f,1.f };

}

void Stage_1_1::Update(float elapsedTime)
{
    Position.y = +20;
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Stage_1_1::Render(RenderContext* rc)
{
    model->render(rc->deviceContext,Transform);
}
