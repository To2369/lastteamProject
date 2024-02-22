#include <WICTextureLoader.h>
using namespace DirectX;
#include <wrl.h>
#include "misc.h"

#include "Graphics/shader.h"
#include "geometric_primitive.h"

geometric_primitive::geometric_primitive(ID3D11Device* device)
{
	// サイズが 1.0 の正立方体データを作成する（重心を原点にする）。
	// 正立方体のコントロールポイント数は 8 個、
	// 1 つのコントロールポイントの位置には法線の向きが違う頂点が
	//  3 個あるので頂点情報の総数は 8x3=24 個、
	// 頂点情報配列（vertices）にすべて頂点の位置・法線情報を格納する。
	vertex vertices[24]{
		//正面
		{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f),DirectX::XMFLOAT3(0,0,-1.0f)},//正面左上 //0
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f),DirectX::XMFLOAT3(0,0,-1.0f)},//正面右上 //1
		{DirectX::XMFLOAT3(0.5f,0.5f,-0.5f),DirectX::XMFLOAT3(0,0,-1.0f)},//正面左下 //2
		{DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f),DirectX::XMFLOAT3(0,0,-1.0f)},//正面右下 //3

		//左
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f),DirectX::XMFLOAT3(-1.0f,0,0)},//左側左上 //4
		{DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f),DirectX::XMFLOAT3(-1.0f,0,0)},//左側右上 //5
		{DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f),DirectX::XMFLOAT3(-1.0f,0,0)},//左側左下 //6
		{DirectX::XMFLOAT3(-0.5f,0.5f,0.5f),DirectX::XMFLOAT3(-1.0f,0,0)},//左側右下 //7

		//後ろ
		{DirectX::XMFLOAT3{-0.5f,-0.5f,0.5f},DirectX::XMFLOAT3(0,0,1.0f)},//後ろ側左側左上 //8
		{DirectX::XMFLOAT3(0.5f,-0.5f,0.5f),DirectX::XMFLOAT3(0,0,1.0f)},//後ろ側右上 //9
		{DirectX::XMFLOAT3(-0.5f,0.5f,0.5f),DirectX::XMFLOAT3(0,0,1.0f)},//後ろ側左下 //10
		{DirectX::XMFLOAT3(0.5f,0.5f,0.5f),DirectX::XMFLOAT3(0,0,1.0f)},//後ろ側右下 //11

		//右
		{DirectX::XMFLOAT3(0.5f,-0.5f,0.5f),DirectX::XMFLOAT3(1.0f,0,0)},//右側左上 //12
		{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f),DirectX::XMFLOAT3(1.0f,0,0)},//右側右上 //13
		{DirectX::XMFLOAT3(0.5f,0.5f,0.5f),DirectX::XMFLOAT3(1.0f,0,0)},//右側左下 //14
		{DirectX::XMFLOAT3(0.5f,0.5f,-0.5f),DirectX::XMFLOAT3(1.0f,0,0)},//右側右下 //15
		
		//下
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f),DirectX::XMFLOAT3(0,-1.0f,0)},//下側左上 //16
		{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f),DirectX::XMFLOAT3(0,-1.0f,0)},//下側右上 //17
		{DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f),DirectX::XMFLOAT3(0,-1.0f,0)},//下側左下 //18
		{DirectX::XMFLOAT3(0.5f,-0.5f,0.5f),DirectX::XMFLOAT3(0,-1.0f,0)},//下側右下 //19

		//上
		{DirectX::XMFLOAT3(-0.5f,0.5f,0.5f),DirectX::XMFLOAT3(0,1.0f,0)},//上側左上 //20
		{DirectX::XMFLOAT3(0.5f,0.5f,0.5f),DirectX::XMFLOAT3(0,1.0f,0)},//上側右上 //21
		{DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f),DirectX::XMFLOAT3(0,1.0f,0)},//上側左下 //22
		{DirectX::XMFLOAT3(0.5f,0.5f,-0.5f),DirectX::XMFLOAT3(0,1.0f,0)},//上側右下 //23
	};

	//正立方体は 6 面持ち、
	// 1 つの面は 2 つの 3 角形ポリゴンで構成されるので 
	// 3 角形ポリゴンの総数は 6x2=12 個、
	// 正立方体を描画するために 12 回の 3 角形ポリゴン描画が必要、よって参照される頂点情報は 12x3=36 回、
	//  3 角形ポリゴンが参照する頂点情報のインデックス（頂点番号）を描画順に配列（indices）に格納する。
	//  時計回りが表面になるように格納すること。

	uint32_t indices[36]{
		0,1,2,	3,2,1,		//正面
		4,5,6,	7,6,5,		//左側
		8,9,10, 11,10,9,	//後ろ側
		12,13,14, 15,14,13, //右側
		16,17,18, 19,18,17, //下側
		20,21,22, 23,22,21 //上側
	};
	create_com_buffers(device, vertices, 24, indices, 36);

	HRESULT hr{ S_OK };

	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	create_vs_from_cso(device, "shader\\geometric_primitive_vs.cso", vertex_shader.GetAddressOf(),
		input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
	create_ps_from_cso(device, "shader\\geometric_primitive_ps.cso", pixel_shader.GetAddressOf());

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(constants);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void geometric_primitive::create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
	HRESULT hr{ S_OK };

	D3D11_BUFFER_DESC buffer_desc{};
	D3D11_SUBRESOURCE_DATA subresorce_data{};
	buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresorce_data.pSysMem = vertices;
	subresorce_data.SysMemPitch = 0;
	subresorce_data.SysMemSlicePitch = 0;
	hr = device->CreateBuffer(&buffer_desc,
		&subresorce_data, 
		vertex_buffer.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	subresorce_data.pSysMem = indices;
	hr = device->CreateBuffer(&buffer_desc, &subresorce_data, index_buffer.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void geometric_primitive::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
	uint32_t stride{ sizeof(vertex) };
	uint32_t offset{ 0 };
	immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediate_context->IASetInputLayout(input_layout.Get());

	immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	constants data{ world,material_color };
	immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
	immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

	D3D11_BUFFER_DESC buffer_desc{};
	index_buffer->GetDesc(&buffer_desc);
	immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
}

