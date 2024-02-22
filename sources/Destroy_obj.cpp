#include"Destroy_obj.h"
using namespace std;
DestroyObj::DestroyObj(ID3D11Device* device)
{

    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::cution);
    //  Position = { 0,0,0 };
    Scale.x = Scale.y = Scale.z = 10.f;

}

DestroyObj::~DestroyObj()
{

}

void DestroyObj::Update(float elapsedTime)
{


    UpdateTransform();
    Return_orijinal_ObjType(elapsedTime);
    ObjType_effect(elapsedTime, Position);
}

void DestroyObj::Render(ID3D11DeviceContext* dc)
{
    model->render(dc, Transform,0.0f,color);
}

