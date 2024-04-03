#include"Stage.h"
using namespace DirectX;
using namespace std;

Stage_1_1::Stage_1_1(ID3D11Device* device)
{
    model = make_unique<Model>(device,filename,true);
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    Scale = { 10.f,2.f,2.f };

}

void Stage_1_1::Update(float elapsedTime)
{
    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Stage_1_1::Render(RenderContext* rc)
{
    model->render(rc->deviceContext,Transform);
}

bool Stage_1_1::raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::intersectRayAndModel(start, end, model, hit);
}