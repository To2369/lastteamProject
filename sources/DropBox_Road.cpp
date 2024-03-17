#include"Gimic.h"
#include"objectManajer.h"
#include"camera.h"
using namespace std;
using namespace DirectX;

DropBox_Road::DropBox_Road(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename,true);
    HitBox = make_unique<Model>(device, filename,true);
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    Scale = { 10.f,10.f,10.f };
    color = {1.f,1.f,1.f,1.f};
  
}
void DropBox_Road::Update(float elapsedTime)
{
  
    Objectmanajer& ince_o = Objectmanajer::incetance();
    Camera& ince_c = Camera::instance();
    int count = ince_o.Get_GameObjCount();
    {
        for (int i = 0; i < count; i++)
        {
            Object* obj = ince_o.Get_GameObject(i);
            if (ince_o.Bounding_Box_vs_Bounding_Box(obj, this, true, 0.045f))
            {
                if (obj->Get_isGimic_UpPosNow())
                {
                    playerStopFlag = true;
                    dropbox.oppnentPos = obj->GetPosition();
                    dropbox.flag = true;
                    dropbox.scaleX = obj->GetScale().x;
                    obj->Set_NotUpdateFlag(true);
                }
            }
            else
            {
                obj->Set_NotUpdateFlag(false);
                dropbox.oppnentPos = {};
                dropbox.flag = false;
            }
        }
        if (dropbox.flag&&isPlayerInRangeOf_Box())
        {
            playerStopFlag = false;
        }
        else playerStopFlag = true;
    }
 
    UpdateTransform();
    HitBox_TransformUpdate();
}
void DropBox_Road::Render(RenderContext* rc)
{
   
    HitBox->render(rc->deviceContext, HitBox_Transform, 0.f, {1,1,1,0.3f});

    model->render(rc->deviceContext, Transform, 0.f, color);
  
}



void DropBox_Road::HitBox_TransformUpdate()
{
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
     { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
     { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
     { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
     { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    const float scale_factor = 0.01f;
    XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
    XMMATRIX S{ XMMatrixScaling(HitBox_Scale.x
                               ,HitBox_Scale.y,
                                HitBox_Scale.z) };
    XMMATRIX R{XMMatrixRotationRollPitchYaw(HitBox_Angle.x,HitBox_Angle.y,HitBox_Angle.z)};
    XMMATRIX T{ XMMatrixTranslation(HitBox_Position.x
                                   ,HitBox_Position.y,
                                    HitBox_Position.z) };

    XMMATRIX World=C*S*R*T;
    XMStoreFloat4x4(&HitBox_Transform, World);
   

}
bool DropBox_Road::isPlayerInRangeOf_Box()
{
    Camera& ince_c = Camera::instance();
    XMFLOAT2 camerapos = { ince_c.GetEye().x,ince_c.GetEye().z };
    XMFLOAT2 boxpos = { dropbox.oppnentPos.x,dropbox.oppnentPos.z};
    XMVECTOR Len =XMVectorSubtract(XMLoadFloat2(&boxpos),XMLoadFloat2(&camerapos));
    float len=XMVectorGetX(XMVector3Length(Len));
    float radius = 0.3f;
    if (len < radius)
    {
        return true;
    }
    return false;
}