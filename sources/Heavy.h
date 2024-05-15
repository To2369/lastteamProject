#pragma once
#include "object.h"
class Heavy :public Object
{
public:
    Heavy(ID3D11Device* device);
    Heavy(ID3D11Device* device,const char*filename_);
    ~Heavy()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
private:

    const char* filename = ".\\resources\\stage1\\tumiki.fbx";
    //const char* filename = ".\\resources\\stage3\\rifuto_B.fbx";

 };
class Super_Heavy :public Object
{
public:
    Super_Heavy(ID3D11Device* device);
    Super_Heavy(ID3D11Device* device,const char*filename_);
    ~Super_Heavy()override;
    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
    void Gui()override;
private:
    const char* filename = ".\\resources\\stage1\\tumiki.fbx";

};
