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
		float dx,float dy,	//��`�̍���̍��W�i�X�N���[�����W�n�j
		float dw,float dh,	//��`�̃T�C�Y�i�X�N���[�����W�n�j
		float r,float g, //red,green
		float b,float a, //blue,�s�����x
		float angle /*degree*/
		);
	void render(ID3D11DeviceContext* immediate_context,
		float dx,float dy,	//��`�̍���̍��W�i�X�N���[�����W�n�j
		float dw,float dh,	//��`�̃T�C�Y�i�X�N���[�����W�n�j
		float r,float g, //red,green
		float b,float a, //blue,�s�����x
		float angle, /*degree*/
		float sx,float sy,//������W
		float sw,float sh//�T�C�Y
		);

	void render(ID3D11DeviceContext* immediate_context, 
		float dx,//��`�̍���̍��W(x)
		float dy,//��`�̍���̍��W(y)
		float dw,//��`�̃T�C�Y(����)
		float dh/*��`�̃T�C�Y(�c��)*/);

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