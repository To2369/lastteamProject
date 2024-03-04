#include "Debug_Stage.h"
//
//DebugStage::DebugStage(ID3D11Device* device)
//{
//    model = make_unique<Model>(device, filename, true, 0.1f);
//    initialaize_Set_attribute(ObjType::null);
//    Scale = { 10.1f,0.1f,10.1f };
//}
//
//void DebugStage::Update(float elapsedTime)
//{
//    UpdateTransform();
//    ObjType_effect(elapsedTime);
//}
//
//void DebugStage::Render(RenderContext* rc)
//{
//    model->render(rc->deviceContext, Transform, 0.0f, GetColor());
//}
//
//bool DebugStage::modelRay_VS_Stage(Intersection& inter, XMFLOAT3 FastPos, XMFLOAT3 normal, float rayLength)
//{
//    return model->Get_RaycastCollition()->raycast(FastPos, normal, Transform, inter, rayLength);
//}
