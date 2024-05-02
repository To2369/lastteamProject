#pragma once
#include"Character.h"
#include"Graphics/RenderContext.h"
#include"model.h"
//���ˊ�
class Syringe : public Character
{
public:
    Syringe(ID3D11Device* device);
    ~Syringe() override;

    //�X�V����
    void update(float elapsedTime);

    //�`�揈��
    void render(RenderContext* rc);

    Model* GetModel() { return model.get(); }

private:

private:
    std::unique_ptr<Model> model;
    const char* filename = ".\\Resources\\Character\\Syringe\\syringe_m.fbx";
};