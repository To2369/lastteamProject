#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "collision_mesh.h"

class VMCFHT
{
public:
	VMCFHT()
		:view_port{}
		, ray_position({ 0,0,0 })
		, ray_direction({ 0,0,0 })
	{};
	~VMCFHT() {};

    static VMCFHT& instance()
	{
		static VMCFHT ins;
		return ins;
	}

	void SetViewPort(D3D11_VIEWPORT& view_port) {
		this->view_port = view_port;
	}

	//ÉJÉÅÉâä÷åW
	void update(DirectX::XMFLOAT4X4& view_projection, DirectX::XMFLOAT4& camera_position);

	//ÉJÉÅÉâà»äO
	void update(DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& distance);

	//raycast
	bool raycast(collision_mesh& collision_mesh, DirectX::XMFLOAT4X4 transform,Intersection& intersection,float length = 1.0e+7f,bool skip = false);

	static DirectX::XMFLOAT3 convert_screen_to_world(LONG x/*screen*/, LONG y/*screen*/, FLOAT z/*ndc*/, D3D11_VIEWPORT vp, const DirectX::XMFLOAT4X4& view_projection);
private:
	D3D11_VIEWPORT view_port;
	DirectX::XMFLOAT3 ray_position;
	DirectX::XMFLOAT3 ray_direction;
};
