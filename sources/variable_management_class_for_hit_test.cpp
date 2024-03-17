#include "variable_management_class_for_hit_test.h"
#include "scene_management.h"

DirectX::XMFLOAT3 VMCFHT::convert_screen_to_world(LONG x/*screen*/, LONG y/*screen*/, FLOAT z/*ndc*/, D3D11_VIEWPORT vp, const DirectX::XMFLOAT4X4& view_projection)
{
	using namespace DirectX;
	XMFLOAT3 position;
	XMStoreFloat3(&position,
		XMVector3TransformCoord(
			XMVector3TransformCoord(
				XMVectorSet(static_cast<float>(x), static_cast<float>(y), z, 1),
				XMMatrixInverse(NULL,
					XMMatrixSet(
						vp.Width * 0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, -vp.Height * 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, vp.MaxDepth - vp.MinDepth, 0.0f,
						vp.TopLeftX + vp.Width * 0.5f, vp.Height * 0.5f + vp.TopLeftY, vp.MinDepth, 1.0f))
			), XMMatrixInverse(NULL, XMLoadFloat4x4(&view_projection))
		)
	);
	return position;
}

//カメラ専用
void VMCFHT::update(DirectX::XMFLOAT4X4& view_projection, DirectX::XMFLOAT4& camera_position)
{
	DirectX::XMFLOAT2 cursor_pos = SceneManagement::instance().GetCursorPosition();

	DirectX::XMFLOAT3 position_on_near_plane = convert_screen_to_world(cursor_pos.x, cursor_pos.y, 0.0f, view_port, view_projection);

	DirectX::XMVECTOR P = XMLoadFloat4(&camera_position);
	DirectX::XMVECTOR D = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&position_on_near_plane) - P);
	DirectX::XMStoreFloat3(&ray_position, P);
	DirectX::XMStoreFloat3(&ray_direction, D);
}

//カメラ以外
void VMCFHT::update(DirectX::XMFLOAT3& pos0, DirectX::XMFLOAT3& normal)
{
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&pos0);
	DirectX::XMVECTOR D = DirectX::XMLoadFloat3(&normal);
	
	DirectX::XMVector3Normalize(D);

	DirectX::XMStoreFloat3(&ray_position, P);
	DirectX::XMStoreFloat3(&ray_direction, D);
}

bool VMCFHT::raycast(collision_mesh& collision_mesh, DirectX::XMFLOAT4X4 transform, Intersection& intersection,float length,bool skip)
{
	
	return collision_mesh.raycast(ray_position, ray_direction, transform, intersection, length, skip);
}

