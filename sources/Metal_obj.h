#pragma once
#include "object.h"
class Metal :public Object
{
public:
    Metal(ID3D11Device* device);
    ~Metal()override;
    void Update(float elapsedTime)override;
    void Render(ID3D11DeviceContext* dc)override;


    const char* GetFbxName()override { return filename; }
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";

};
