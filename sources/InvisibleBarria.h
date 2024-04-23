#pragma once

#include"Stage.h"

//Œ©‚¦‚È‚¢•Çobject
class InvisibleBarrier :public Static_Object
{
public:

    InvisibleBarrier(ID3D11Device* device);
    ~InvisibleBarrier() override {}
public:
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";

};
