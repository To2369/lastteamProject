#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "collision_mesh.h"
#include"Model.h"
enum class Ray_ObjType//どのオブジェクトに対してRayを飛ばすかを指定
{
	Static_objects,//静的オブジェクト
	DaynamicObjects,//動的オブジェクト
	DynamicGimics,//動的ギミックオブジェ
	Stage,
};
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

	//カメラ関係
	void update(DirectX::XMFLOAT4X4& view_projection, DirectX::XMFLOAT4& camera_position);

	//カメラ以外
	void update(DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& distance);

	//raycast
	bool raycast(collision_mesh& collision_mesh, DirectX::XMFLOAT4X4 transform, Intersection& intersection, float length = 1.0e+7f, bool skip = false);
	bool raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result, DirectX::XMFLOAT4X4 WorldTransform_, DirectX::XMMATRIX rotation_ = DirectX::XMMatrixIdentity()/*回転だけ適用するための行列 */, bool rotation_flag=false/*rotation_をworld行列に適用するかのフラグ */);
	bool moveStage_raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,  HitResult& result,vector<Model::mesh>&meshes);
	bool RayCast(DirectX::XMFLOAT3 Start, DirectX::XMFLOAT3 End, HitResult& hit, Ray_ObjType type);
	static DirectX::XMFLOAT3 convert_screen_to_world(LONG x/*screen*/, LONG y/*screen*/, FLOAT z/*ndc*/, D3D11_VIEWPORT vp, const DirectX::XMFLOAT4X4& view_projection);
private:
	D3D11_VIEWPORT view_port;
	DirectX::XMFLOAT3 ray_position;
	DirectX::XMFLOAT3 ray_direction;
};
