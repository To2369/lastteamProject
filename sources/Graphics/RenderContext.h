#pragma once

#include <d3d11.h>
#include <directxmath.h>

enum class rasterizerMode
{
	Sorid_uramen_off,// ソリッド描画(裏面非表示)
	Wireframe_uramen_off,//ワイヤーフレーム描画(裏面非表示)
	Sorid_ryoumen_on,// ソリッド描画(両面表示)
	Wirefream_ryoumen_on// ワイヤーフレーム(両面表示)

};
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;
  
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4 lightDirection;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizers[4];
   	
};
