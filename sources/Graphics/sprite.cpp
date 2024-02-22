#include <WICTextureLoader.h>
using namespace DirectX;
#include <wrl.h>
#include "misc.h"
#include <sstream>
#include "texture.h"
#include "Graphics/shader.h"
#include "sprite.h"

inline void rotate(float& x, float& y, float cx, float cy, float angle)
{
	x -= cx;
	y -= cy;

	float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
	float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
	float tx{ x }, ty{ y };
	//(x) (cosθ,-sinθ,0)(x)
	//(y)=(sinθ, cosθ,0)(y)
	//(1) (0   , 0,   1)(1)
	x = cos * tx + -sin * ty;
	y = sin * tx + cos * ty;

	x += cx;
	y += cy;
};


inline void position(float& x, float& y, float w, float h)
{
	x = 2.0f * x / w - 1.0f;
	y = 1.0f - 2.0f * y / h;
};

void sprite::render(ID3D11DeviceContext* immediate_context,
	float dx,float dy,float dw,float dh,
	float r,float g,float b,float a,float angle)
{
	D3D11_VIEWPORT viewport{};
	UINT num_viewports{ 1 };
	immediate_context->RSGetViewports(&num_viewports, &viewport);

	// (x0, y0) *----* (x1, y1) 
	//			|   /|
	//			|  / |
	//			| /  |
	//			|/   |
	// (x2, y2) *----* (x3, y3) 

	//left-top
	float x0{ dx };
	float y0{ dy };
	//right-top
	float x1{ dx + dw };
	float y1{ dy };
	//left-bottom
	float x2{ dx };
	float y2{ dy + dh };
	//right-bottom
	float x3{ dx + dw };
	float y3{ dy + dh };


	//回転の中心を矩形の中心点にした場合
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	rotate(x0, y0, cx, cy, angle);
	rotate(x1, y1, cx, cy, angle);
	rotate(x2, y2, cx, cy, angle);
	rotate(x3, y3, cx, cy, angle);



	position(x0, y0, viewport.Width,viewport.Height);
	position(x1, y1, viewport.Width,viewport.Height);
	position(x2, y2, viewport.Width,viewport.Height);
	position(x3, y3, viewport.Width,viewport.Height);

	//x0 = 2.0f * x0 / viewport.Width - 1.0f;
	//y0 = 1.0f - 2.0f * y0 / viewport.Height;
	//x1 = 2.0f * x1 / viewport.Width - 1.0f;
	//y1 = 1.0f - 2.0f * y1 / viewport.Height;
	//x2 = 2.0f * x2 / viewport.Width - 1.0f;
	//y2 = 1.0f - 2.0f * y2 / viewport.Height;
	//x3 = 2.0f * x3 / viewport.Width - 1.0f;
	//y3 = 1.0f - 2.0f * y3 / viewport.Height;


	HRESULT hr{ S_OK };
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		vertices[0].position = { x0,y0,0 };
		vertices[1].position = { x1,y1,0 };
		vertices[2].position = { x2,y2,0 };
		vertices[3].position = { x3,y3,0 };
		vertices[0].color = 
			vertices[1].color = 
			vertices[2].color = 
			vertices[3].color = { r,g,b,a };

		vertices[0].texcoord = { 0,0 };
		vertices[1].texcoord = { 1,0 };
		vertices[2].texcoord = { 0,1 };
		vertices[3].texcoord = { 1,1 };
	}

	immediate_context->Unmap(vertex_buffer.Get(), 0);

	UINT stride{ sizeof(vertex) };
	UINT offset{ 0 };
	immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	immediate_context->IASetInputLayout(input_layout.Get());

	immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	immediate_context->PSSetShaderResources(0, 1, shader_resouce_view.GetAddressOf());

	immediate_context->Draw(4, 0);
}

//テクセル座標からUV座標への変換
//U = 使用したいテクスチャの位置 / テクスチャの横幅 
//V = 使用したいテクスチャの位置 / テクスチャの縦幅
void sprite::render(ID3D11DeviceContext* immediate_context,
	float dx, float dy, float dw, float dh, 
	float r, float g, float b, float a, float angle, 
	float sx, float sy, float sw, float sh)
{
	D3D11_VIEWPORT viewport{};
	UINT num_viewports{ 1 };
	immediate_context->RSGetViewports(&num_viewports, &viewport);

	// (x0, y0) *----* (x1, y1) 
	//			|   /|
	//			|  / |
	//			| /  |
	//			|/   |
	// (x2, y2) *----* (x3, y3) 

	//left-top
	float x0{ dx };
	float y0{ dy };
	//right-top
	float x1{ dx + dw };
	float y1{ dy };
	//left-bottom
	float x2{ dx };
	float y2{ dy + dh };
	//right-bottom
	float x3{ dx + dw };
	float y3{ dy + dh };

	//回転の中心を矩形の中心点にした場合
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	rotate(x0, y0, cx, cy, angle);
	rotate(x1, y1, cx, cy, angle);
	rotate(x2, y2, cx, cy, angle);
	rotate(x3, y3, cx, cy, angle);

	position(x0, y0, viewport.Width, viewport.Height);
	position(x1, y1, viewport.Width, viewport.Height);
	position(x2, y2, viewport.Width, viewport.Height);
	position(x3, y3, viewport.Width, viewport.Height);

	//x0 = 2.0f * x0 / viewport.Width - 1.0f;
	//y0 = 1.0f - 2.0f * y0 / viewport.Height;
	//x1 = 2.0f * x1 / viewport.Width - 1.0f;
	//y1 = 1.0f - 2.0f * y1 / viewport.Height;
	//x2 = 2.0f * x2 / viewport.Width - 1.0f;
	//y2 = 1.0f - 2.0f * y2 / viewport.Height;
	//x3 = 2.0f * x3 / viewport.Width - 1.0f;
	//y3 = 1.0f - 2.0f * y3 / viewport.Height;

	//sub
	//float U0 = 0;
	//float V0 = 0;
	//float U1 = 1;
	//float V1 = 0;
	//float U2 = 0;
	//float V2 = 1;
	//float U3 = 1;
	//float V3 = 1;

	//縦14 横5 player

	float w = texture2d_desc.Width;
	float h = texture2d_desc.Height;

	float U0{ sx / w };
	float V0{ sy / h };
	float U1{ (sx + sw) / w };
	float V1{ sy / h };
	float U2{ sx / w };
	float V2{ (sy + sh) / h };
	float U3{ (sx + sw) / w };
	float V3{ (sy + sh) / h };

	HRESULT hr{ S_OK };
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		vertices[0].position = { x0,y0,0 };
		vertices[1].position = { x1,y1,0 };
		vertices[2].position = { x2,y2,0 };
		vertices[3].position = { x3,y3,0 };
		vertices[0].color =
			vertices[1].color =
			vertices[2].color =
			vertices[3].color = { r,g,b,a };

		vertices[0].texcoord = { U0,V0 };
		vertices[1].texcoord = { U1,V1 };
		vertices[2].texcoord = { U2,V2 };
		vertices[3].texcoord = { U3,V3 };
	}
	
	immediate_context->Unmap(vertex_buffer.Get(), 0);

	UINT stride{ sizeof(vertex) };
	UINT offset{ 0 };
	immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	immediate_context->IASetInputLayout(input_layout.Get());

	immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	immediate_context->PSSetShaderResources(0, 1, shader_resouce_view.GetAddressOf());

	immediate_context->Draw(4, 0);
}

void sprite::render(ID3D11DeviceContext* immediate_context, 
	float dx, float dy, 
	float dw, float dh)
{
	D3D11_VIEWPORT viewport{};
	UINT num_viewports{ 1 };
	immediate_context->RSGetViewports(&num_viewports, &viewport);

	//left-top
	float x0{ dx };
	float y0{ dy };
	//right-top
	float x1{ dx + dw };
	float y1{ dy };
	//left-bottom
	float x2{ dx };
	float y2{ dy + dh };
	//right-bottom
	float x3{ dx + dw };
	float y3{ dy + dh };

	position(x0, y0, viewport.Width, viewport.Height);
	position(x1, y1, viewport.Width, viewport.Height);
	position(x2, y2, viewport.Width, viewport.Height);
	position(x3, y3, viewport.Width, viewport.Height);

	HRESULT hr{ S_OK };
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		vertices[0].position = { x0,y0,1 };
		vertices[1].position = { x1,y1,1 };
		vertices[2].position = { x2,y2,1 };
		vertices[3].position = { x3,y3,1 };
		vertices[0].color =
			vertices[1].color =
			vertices[2].color =
			vertices[3].color = { 1,1,1,1 };

		vertices[0].texcoord = { 0,0 };
		vertices[1].texcoord = { 1,0 };
		vertices[2].texcoord = { 0,1 };
		vertices[3].texcoord = { 1,1 };
	}

	immediate_context->Unmap(vertex_buffer.Get(), 0);

	UINT stride{ sizeof(vertex) };
	UINT offset{ 0 };
	immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	immediate_context->IASetInputLayout(input_layout.Get());

	immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	immediate_context->PSSetShaderResources(0, 1, shader_resouce_view.GetAddressOf());

	immediate_context->Draw(4, 0);
}

void sprite::textout(ID3D11DeviceContext* immediate_context, 
	std::string s, 
	float x, float y, float w, float h, 
	float r, float g, float b, float a)
{
	float sw = static_cast<float>(texture2d_desc.Width / 16);
	float sh = static_cast<float>(texture2d_desc.Height / 16);
	float carriage = 0;
	for (const char c : s)
	{
		render(immediate_context, x + carriage, y,
			w, h, r, g, b, a, 0, 
			sw * (c & 0x0f),sh * (c >> 4), 
			sw, sh);
		carriage += w;
	}
}

sprite::sprite(ID3D11Device* device, const wchar_t* filename)
{
	vertex vertices[]
	{
		{ {-1.0,+1.0,0},{1,1,1,1},{0,0} },
		{ {+1.0,+1.0,0},{1,1,1,1},{1,0} },
		{ {-1.0,-1.0,0},{1,1,1,1},{0,1} },
		{ {+1.0,-1.0,0},{1,1,1,1},{1,1} },
	};

	HRESULT hr{ S_OK };

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertices);
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA subresouce_data{};
	subresouce_data.pSysMem = vertices;
	subresouce_data.SysMemPitch = 0;
	subresouce_data.SysMemSlicePitch = 0;
	hr = device->CreateBuffer(&buffer_desc, &subresouce_data, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	const char* cso_name{ "shader\\sprite_vs.cso" };

	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	hr = create_vs_from_cso(device, cso_name, vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc,_countof(input_element_desc));

	cso_name = "shader\\sprite_ps.cso";
	
	hr = create_ps_from_cso(device, cso_name, pixel_shader.GetAddressOf());

	hr = load_texture_from_file(device, filename, shader_resouce_view.GetAddressOf(), &texture2d_desc);
}

sprite::~sprite()
{
	release_all_textures();
}
