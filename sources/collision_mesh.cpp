#include "misc.h"
#include "collision_mesh.h"

#include <sstream>
#include <functional>

using namespace DirectX;

inline XMFLOAT4X4 to_xmfloat4x4(const FbxAMatrix& fbxamatrix)
{
	XMFLOAT4X4 xmfloat4x4;
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
		}
	}
	return xmfloat4x4;
}

collision_mesh::collision_mesh(ID3D11Device* device, const char* fbx_filename, bool triangulate)
{
	FbxManager* fbx_manager = FbxManager::Create();
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "");
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	bool import_status = false;
	import_status = fbx_importer->Initialize(fbx_filename);
	assert(import_status && "Failed to call FbxImporter::Initialize");
	import_status = fbx_importer->Import(fbx_scene);
	assert(import_status && "Failed to call FbxImporter::Import");

	FbxGeometryConverter fbx_converter(fbx_manager);
	if (triangulate)
	{
		fbx_converter.Triangulate(fbx_scene, true/*replace*/, false/*legacy*/);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);
	}

	std::function<void(FbxNode*)> traverse = [&](FbxNode* fbx_node) {
		if (fbx_node->GetNodeAttribute() && fbx_node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			
			decltype(meshes)::reference mesh = meshes.emplace_back();
			mesh.name = fbx_node->GetName();

			XMMATRIX global_transform = XMLoadFloat4x4(&to_xmfloat4x4(fbx_node->EvaluateGlobalTransform()));

			FbxMesh* fbx_mesh = fbx_node->GetMesh();
			const int material_count = fbx_mesh->GetNode()->GetMaterialCount();
			mesh.subsets.resize(material_count > 0 ? material_count : 1);
			for (int material_index = 0; material_index < material_count; ++material_index)
			{
				const FbxSurfaceMaterial* fbx_material = fbx_mesh->GetNode()->GetMaterial(material_index);
				mesh.subsets.at(material_index).material_name = fbx_material->GetName();
			}

			const FbxVector4* control_points = fbx_mesh->GetControlPoints();
			const int polygon_count = fbx_mesh->GetPolygonCount();
			for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
			{
				const int material_index = material_count > 0 ? fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) : 0;
				decltype(mesh.subsets)::reference subset = mesh.subsets.at(material_index);

				for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon)
				{
					DirectX::XMFLOAT3 position;
					const int polygon_vertex = fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon);
					position.x = static_cast<float>(control_points[polygon_vertex][0]);
					position.y = static_cast<float>(control_points[polygon_vertex][1]);
					position.z = static_cast<float>(control_points[polygon_vertex][2]);
					XMStoreFloat3(&position, XMVector3TransformCoord(XMLoadFloat3(&position), global_transform));
					subset.positions.emplace_back(position);
				}

			}
			for (decltype(mesh.subsets)::const_reference subset : mesh.subsets)
			{
				for (decltype(subset.positions)::const_reference position : subset.positions)
				{
					mesh.bounding_box[0].x = std::min<float>(mesh.bounding_box[0].x, position.x);
					mesh.bounding_box[0].y = std::min<float>(mesh.bounding_box[0].y, position.y);
					mesh.bounding_box[0].z = std::min<float>(mesh.bounding_box[0].z, position.z);
					mesh.bounding_box[1].x = std::max<float>(mesh.bounding_box[1].x, position.x);
					mesh.bounding_box[1].y = std::max<float>(mesh.bounding_box[1].y, position.y);
					mesh.bounding_box[1].z = std::max<float>(mesh.bounding_box[1].z, position.z);
				}
			}
		}
		for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index)
		{
			traverse(fbx_node->GetChild(child_index));
		}
	};
	traverse(fbx_scene->GetRootNode());

	fbx_manager->Destroy();
}
inline bool intersect_ray_aabb(const float p[3], const float d[3], const float p0[3], const float p1[3])
{
	float t_min = 0;
	float t_max = +FLT_MAX;

	for (size_t a = 0; a < 3; ++a)
	{
		float inv_d = 1.0f / d[a];
		float t0 = (p0[a] - p[a]) * inv_d;
		float t1 = (p1[a] - p[a]) * inv_d;
		if (inv_d < 0.0f)
		{
			std::swap<float>(t0, t1);
		}
		t_min = std::max<float>(t0, t_min);
		t_max = std::min<float>(t1, t_max);

		if (t_max <= t_min)
		{
			return false;
		}
	}
	return true;
}
// The coordinate system of all function arguments is world space.
bool collision_mesh::raycast(_In_ XMFLOAT3 ray_position, _In_ XMFLOAT3 ray_direction, _In_ const XMFLOAT4X4& transform, _Out_ Intersection& intersection, _In_ float ray_length_limit, _In_ bool skip_if) const
{
	XMMATRIX T = XMLoadFloat4x4(&transform);
	XMMATRIX _T = XMMatrixInverse(NULL, T);

	// Convert to model space.
	XMStoreFloat3(&ray_position, XMVector3TransformCoord(XMLoadFloat3(&ray_position), _T));
	XMStoreFloat3(&ray_direction, XMVector3TransformNormal(XMLoadFloat3(&ray_direction), _T));

	const XMVECTOR P = XMLoadFloat3(&ray_position);
	const XMVECTOR D = XMVector3Normalize(XMLoadFloat3(&ray_direction));

	int intersection_count = 0;
	float closest_distance = FLT_MAX;

	for (decltype(meshes)::const_reference mesh : meshes)
	{
#if 1
		const float* aabb_min = reinterpret_cast<const float*>(&mesh.bounding_box[0]);
		const float* aabb_max = reinterpret_cast<const float*>(&mesh.bounding_box[1]);
		if (!intersect_ray_aabb(reinterpret_cast<const float*>(&ray_position), reinterpret_cast<const float*>(&ray_direction), aabb_min, aabb_max))
		{
			continue;
		}
#endif
		for (decltype(mesh.subsets)::const_reference subset : mesh.subsets)
		{
			const XMFLOAT3* positions = subset.positions.data();
			const size_t triangle_count = subset.positions.size() / 3;
			for (size_t triangle_index = 0; triangle_index < triangle_count; triangle_index++)
			{
				const XMVECTOR A = XMLoadFloat3(&positions[triangle_index * 3 + 0]);
				const XMVECTOR B = XMLoadFloat3(&positions[triangle_index * 3 + 1]);
				const XMVECTOR C = XMLoadFloat3(&positions[triangle_index * 3 + 2]);

				const XMVECTOR N = XMVector3Normalize(XMVector3Cross(XMVectorSubtract(B, A), XMVectorSubtract(C, A)));
				const float d = XMVectorGetByIndex(XMVector3Dot(N, A), 0);
				const float denominator{ XMVectorGetByIndex(XMVector3Dot(N, D), 0) };
				if (denominator < 0) // Note that if N.D = 0 , then D is parallel to the plane and the ray does not intersect the plane.
				{
					const float numerator = d - XMVectorGetByIndex(XMVector3Dot(N, P), 0);
					const float t = numerator / denominator;

					if (t > 0 && t < ray_length_limit) // Forward and Length limit of Ray
					{
						XMVECTOR Q = XMVectorAdd(P, XMVectorScale(D, t));

						const XMVECTOR QA = XMVectorSubtract(A, Q);
						const XMVECTOR QB = XMVectorSubtract(B, Q);
						const XMVECTOR QC = XMVectorSubtract(C, Q);

						XMVECTOR U = XMVector3Cross(QB, QC);
						XMVECTOR V = XMVector3Cross(QC, QA);
						if (XMVectorGetByIndex(XMVector3Dot(U, V), 0) < 0)
						{
							continue;
						}

						XMVECTOR W = XMVector3Cross(QA, QB);
						if (XMVectorGetByIndex(XMVector3Dot(U, W), 0) < 0)
						{
							continue;
						}
						if (XMVectorGetByIndex(XMVector3Dot(V, W), 0) < 0)
						{
							continue;
						}
						intersection_count++;

						if (t < closest_distance)
						{
							closest_distance = t;

							XMStoreFloat3(&intersection.intersection_position, XMVector3TransformCoord(Q, T));
							XMStoreFloat3(&intersection.intersection_normal, XMVector3Normalize(XMVector3TransformNormal(N, T)));
							intersection.intersection_mesh = mesh.name;
							intersection.intersection_material = subset.material_name;

							if (skip_if)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return intersection_count > 0;
}