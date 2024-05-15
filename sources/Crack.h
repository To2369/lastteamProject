#pragma once
#include"object.h"

class Crack :public Object
{
public:
    Crack(ID3D11Device* device);
    Crack(ID3D11Device* device,const char*filename_);
    ~Crack()override{};
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;

private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
};
