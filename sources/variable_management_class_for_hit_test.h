#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "collision_mesh.h"
#include"Model.h"
enum class Ray_ObjType//�ǂ̃I�u�W�F�N�g�ɑ΂���Ray���΂������w��
{
	Static_objects,//�ÓI�I�u�W�F�N�g
	DaynamicObjects,//���I�I�u�W�F�N�g
	DynamicGimics,//���I�M�~�b�N�I�u�W�F
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

	//�J�����֌W
	void update(DirectX::XMFLOAT4X4& view_projection, DirectX::XMFLOAT4& camera_position);

	//�J�����ȊO
	void update(DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& distance);

	//raycast
	bool raycast(collision_mesh& collision_mesh, DirectX::XMFLOAT4X4 transform, Intersection& intersection, float length = 1.0e+7f, bool skip = false);
	bool raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result, DirectX::XMFLOAT4X4 WorldTransform_, DirectX::XMMATRIX rotation_ = DirectX::XMMatrixIdentity()/*��]�����K�p���邽�߂̍s�� */, bool rotation_flag=false/*rotation_��world�s��ɓK�p���邩�̃t���O */);
	bool moveStage_raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,  HitResult& result,vector<Model::mesh>&meshes);
	bool RayCast(DirectX::XMFLOAT3 Start, DirectX::XMFLOAT3 End, HitResult& hit, Ray_ObjType type);
	static DirectX::XMFLOAT3 convert_screen_to_world(LONG x/*screen*/, LONG y/*screen*/, FLOAT z/*ndc*/, D3D11_VIEWPORT vp, const DirectX::XMFLOAT4X4& view_projection);
private:
	D3D11_VIEWPORT view_port;
	DirectX::XMFLOAT3 ray_position;
	DirectX::XMFLOAT3 ray_direction;
};
