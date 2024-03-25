#pragma once
#include "object.h"
class Heavy :public Object
{
public:
    Heavy(ID3D11Device* device);
    ~Heavy()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    bool gimic_VS_Object();
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";

};
class Super_Heavy :public Object
{
public:
    Super_Heavy(ID3D11Device* device);
    ~Super_Heavy()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;

private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";

};
