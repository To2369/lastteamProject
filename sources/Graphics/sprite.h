#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class sprite 
{
public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resouce_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;

	void render(ID3D11DeviceContext* immediate_context,
		float dx,float dy,	//矩形の左上の座標（スクリーン座標系）
		float dw,float dh,	//矩形のサイズ（スクリーン座標系）
		float r,float g, //red,green
		float b,float a, //blue,不透明度
		float angle /*degree*/
		);
	void render(ID3D11DeviceContext* immediate_context,
		float dx,float dy,	//矩形の左上の座標（スクリーン座標系）
		float dw,float dh,	//矩形のサイズ（スクリーン座標系）
		float r,float g, //red,green
		float b,float a, //blue,不透明度
		float angle, /*degree*/
		float sx,float sy,//左上座標
		float sw,float sh//サイズ
		);

	void render(ID3D11DeviceContext* immediate_context, 
		float dx,//矩形の左上の座標(x)
		float dy,//矩形の左上の座標(y)
		float dw,//矩形のサイズ(横幅)
		float dh/*矩形のサイズ(縦幅)*/);

	void textout(ID3D11DeviceContext* immediate_context,
		std::string s,
		float x, float y,
		float w, float h, 
		float r, float g,
		float b, float a);

	sprite(ID3D11Device* device,const wchar_t* filename);
	~sprite();

	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};
};