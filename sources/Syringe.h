#pragma once
#include"Character.h"
//’ËŠí
class Syringe : public Character
{
public:
    Syringe(ID3D11Device* device);
    ~Syringe() override;

    //XVˆ—
    void update(float elapsedTime);

    //•`‰æˆ—
    void render(RenderContext* rc);

    Model* GetModel() { return model.get(); }

private:

private:
    std::unique_ptr<Model> model;
    const char* filename = ".\\Resources\\Character\\Hand\\hand.fbx";
};