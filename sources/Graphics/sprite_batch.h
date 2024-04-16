#pragma once
#include <d3d11.h>

#include <wrl.h>
#include <DirectXMath.h>

#include <vector>

class sprite_batch
{
public:

	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;

	const size_t max_vertices;
	std::vector<vertex> vertices;

	void begin(ID3D11DeviceContext* immediate_context,
		ID3D11PixelShader* replaced_pixel_shader = nullptr/*UNIT.10*/,
		ID3D11ShaderResourceView* replaced_shader_resource_view = nullptr/*UNIT.10*/);
	void end(ID3D11DeviceContext* immediate_context);

	void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy,	//矩形の左上の座標（スクリーン座標系）
		float dw, float dh,	//矩形のサイズ（スクリーン座標系）
		float r, float g, //red,green
		float b, float a, //blue,不透明度
		float angle /*degree*/
	);
	void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy,	//矩形の左上の座標（スクリーン座標系）
		float dw, float dh,	//矩形のサイズ（スクリーン座標系）
		float r, float g, //red,green
		float b, float a, //blue,不透明度
		float angle, /*degree*/
		float sx, float sy,//左上座標
		float sw, float sh//サイズ
	);
	void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy,	//矩形の左上の座標（スクリーン座標系）
		float dw, float dh	//矩形のサイズ（スクリーン座標系）
	);


	sprite_batch(ID3D11Device* device, const wchar_t* filename,size_t max_sprites);
	~sprite_batch();
};