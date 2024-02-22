#pragma once

#include <d3d11.h>
#include <directxmath.h>

struct RenderContext
{
	ID3D11DeviceContext* deviceContext;
  
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4 lightDirection;
   	
};
