#include"Cution.h"
#include <Graphics/shader.h>
using namespace std;
#include"objectManajer.h"
Cution::Cution(ID3D11Device* device)
{
    
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };
}

Cution::~Cution()
{

}

void Cution::Update(float elapsedTime)
{
    if (NotUpdateFlag)
    {
        Return_orijinal_ObjType(elapsedTime);
        ObjType_effect(elapsedTime);
        UpdateTransform();
        return;
    }
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (isGimic_UpPosNow)Velocty.y = 0.f;
    else if (!isGimic_UpPosNow)RayCastGround();

    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Cution::Render(RenderContext* rc)
{
    
	model->render(rc->deviceContext, Transform, 0.0f,color);
	rc->deviceContext->RSSetState(rc->rasterizers[3].Get());
    //model->render(rc->deviceContext, UpdateBounding_Box() , 0.0f,color);
	rc->deviceContext->RSSetState(rc->rasterizers[2].Get());
	
}

Super_Cution::Super_Cution(ID3D11Device* device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::Super_cution, ObjType::null);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;
    Normal = { 0.f,-1.f,0.f };

}

Super_Cution::~Super_Cution()
{
}

void Super_Cution::Update(float elapsedTime)
{
    if (NotUpdateFlag)
    {
        Return_orijinal_ObjType(elapsedTime);
        ObjType_effect(elapsedTime);
        UpdateTransform();
        Velocty.y = 0;
        return;
    }
    Return_orijinal_ObjType(elapsedTime);
    Velocty.y = -elapsedTime;
    //box_Collition_obj();

    if (isGimic_UpPosNow)Velocty.y = 0.f;
    else if (!isGimic_UpPosNow)RayCastGround();

    ObjType_effect(elapsedTime);
    UpdateTransform();
}

void Super_Cution::Render(RenderContext* rc)
{
    
    model->render(rc->deviceContext, Transform, 0.0f, color);
}
