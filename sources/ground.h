#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "object.h"
#include "collision_mesh.h"

class Ground : public Object
{
public:
	Ground(ID3D11Device* device);
	~Ground() = default;

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc)override;

	ObjType Get_attribute()override { return attribute_state; }
	const char* GetFbxName()override { return filename; }

private:
	const char* filename = ".\\resources\\3Dmodel\\ExampleStage.fbx";
};