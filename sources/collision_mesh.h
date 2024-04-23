#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <directxmath.h>

#include <vector>
#include <string>

#include <fbxsdk.h>

#include <unordered_map>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>
using namespace std;
using namespace DirectX;

struct Intersection
{
	XMFLOAT3 intersection_position = {};
	XMFLOAT3 intersection_normal = {};
	DirectX::XMFLOAT4X4 transform = {};
	string intersection_mesh{};
	string intersection_material{};
};
class collision_mesh
{
public:

	struct mesh
	{
		std::string name;
		struct subset
		{
			std::string material_name;
			std::vector<DirectX::XMFLOAT3> positions;
		};
		std::vector<subset> subsets;
		DirectX::XMFLOAT3 bounding_box[2]
		{
			{ +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
			{ -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
		};
	};
	std::vector<mesh> meshes;

	collision_mesh(ID3D11Device* device, const char* fbx_filename, bool triangulate = false);

	// The coordinate system of all function arguments is world space.
	bool raycast(_In_ DirectX::XMFLOAT3 ray_position, _In_ DirectX::XMFLOAT3 ray_direction, _In_ const DirectX::XMFLOAT4X4& transform,
		_Out_  Intersection& intersection, _In_ float ray_length_limit = 1.0e+7f, _In_ bool skip_if = false/*Once the first intersection is found, the process is interrupted.*/) const;
};
