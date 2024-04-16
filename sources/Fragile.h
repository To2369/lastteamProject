#pragma once
#include "object.h"
class Fragile :public Object
{

public:
    Fragile(ID3D11Device* device);
    ~Fragile()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
  
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";


};
class Super_fragile :public Object
{

public:
    Super_fragile(ID3D11Device* device);
    ~Super_fragile()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;

private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";


};