#pragma once

#include "object.h"
class Hard_to_Break :public Object
{

public:
    Hard_to_Break(ID3D11Device* device);
    Hard_to_Break(ID3D11Device* device, const char* filename_);
    ~Hard_to_Break()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";


};
class Super_hard_to_Break :public Object
{

public:
    Super_hard_to_Break(ID3D11Device* device);
    Super_hard_to_Break(ID3D11Device* device,const char*filename_);
    ~Super_hard_to_Break()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";


};
