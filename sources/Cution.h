#pragma once
#include "object.h"
class Cution :public Object
{

public:
    Cution(ID3D11Device* device);
    ~Cution()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
    


private:
    const char* filename = ".\\resources\\stage1\\ball.fbx";


};
class Super_Cution :public Object
{

public:
    Super_Cution(ID3D11Device* device);
    ~Super_Cution()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;

private:
    const char* filename = ".\\resources\\stage1\\ball.fbx";


};
