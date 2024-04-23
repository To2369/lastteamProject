#pragma once

#include <d3d11.h>
#include <directxmath.h>

enum class rasterizerMode
{
	Sorid_uramen_off,// �\���b�h�`��(���ʔ�\��)
	Wireframe_uramen_off,//���C���[�t���[���`��(���ʔ�\��)
	Sorid_ryoumen_on,// �\���b�h�`��(���ʕ\��)
	Wirefream_ryoumen_on// ���C���[�t���[��(���ʕ\��)

};
struct RenderContext
{
	//debugdraw�g�����ߒǉ�
	static RenderContext& incetance()
	{
		static RenderContext ince;
		return ince;
	}
	ID3D11DeviceContext* deviceContext;
	ID3D11Device* device;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4 lightDirection;



};
