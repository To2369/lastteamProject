#include"Syringe.h"
#include"PlayerManager.h"
//�R���X�g���N�^
Syringe::Syringe(ID3D11Device* device)
{
    model = std::make_unique<Model>(device, filename, true);
    const float scale_fcator = 0.01f;	//���f�����傫���̂ŃX�P�[������
    scale = { scale_fcator, scale_fcator, scale_fcator };
    color = { 0,0,0,0 };
}

//�f�X�g���N�^
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