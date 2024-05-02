#include"Syringe.h"
#include"PlayerManager.h"
//コンストラクタ
Syringe::Syringe(ID3D11Device* device)
{
    model = std::make_unique<Model>(device, filename, true);
    const float scale_fcator = 0.01f;	//モデルが大きいのでスケール調整
    scale = { scale_fcator, scale_fcator, scale_fcator };
    color = { 0,0,0,0 };
}

//デストラクタ
Syringe::~Syringe()
{

}

void Syringe::update(float elapsedTime)
{

    
}

void Syringe::render(RenderContext* rc)
{
    //model->render(rc->deviceContext, transform, 0, color);
}