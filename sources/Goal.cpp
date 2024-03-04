#include"Gimic.h"
#include"camera.h"
Goal::Goal(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    Gimic_type = Gimic_Type::Goal;
    Scale = { 0.05f,0.05f,0.05f };
    initialaize_Set_attribute(ObjType::null, ObjType::null);
}
void Goal::Update(float elapsedTime)
{
    Goal_Flag = false;
    if (GoalInPosition())
    {
        Goal_Flag = true;
    }
    UpdateTransform();
}

bool Goal::GoalInPosition()
{
    Camera& ince_c = Camera::instance();
    XMVECTOR cameraPos = XMLoadFloat3(&ince_c.GetEye());
    XMVECTOR GoalPos = XMLoadFloat3(&Position);

    XMVECTOR pos = XMVectorSubtract(GoalPos, cameraPos);
    float len = XMVectorGetX(XMVector3Length(pos));
    if (len < radius + radius)
    {
        return true;
    }
    return false;
}

void Goal::Render(RenderContext* rc)
{
    color = { 1,0,0,1 };
    model->render(rc->deviceContext, Transform, 0.0f, color);
}