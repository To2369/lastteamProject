#pragma once
#include "object.h"
class DestroyObj :public Object
{

public:
    DestroyObj(ID3D11Device* device);
    ~DestroyObj()override;
    void Update(float elapsedTime)override;
    void Render(ID3D11DeviceContext* dc)override;

    void Set() {}
    const char* GetFbxName()override { return filename; }
private:
    const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";


};
