#pragma once
#define NOMINMAX

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

#include <vector>
#include <unordered_map>

#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "../tinygltf-release/tiny_gltf.h"

class gltf_model_static_batching
{
	std::string filename;
public:
	gltf_model_static_batching(ID3D11Device* dc, const std::string& filename);
	virtual ~gltf_model_static_batching() = default;

	struct scene
	{
		std::string name;
		std::vector<int> nodes;	//Array of 'root' nodes
	};
	std::vector<scene> scenes;

	struct node
	{
		std::string name;
		int skin{ -1 }; // index of skin references by this node
		int mesh{ -1 }; // index of mesh references by this node

		std::vector<int> children; //An array indices of child nodes of this node

		// Local transforms
		DirectX::XMFLOAT4 rotation{ 0,0,0,1 };
		DirectX::XMFLOAT3 scale{ 1,1,1 };
		DirectX::XMFLOAT3 translation{ 0,0,0 };

		DirectX::XMFLOAT4X4 global_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	};
	std::vector<node> nodes;

	struct buffer_view
	{
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		size_t stride_in_bytes{ 0 };
		size_t size_in_bytes{ 0 };
		size_t count() const
		{
			return size_in_bytes / stride_in_bytes;
		}
		buffer_view() = default;
	};

	struct primitive
	{
		int material;
		std::map<std::string, buffer_view> vertex_buffer_views;
		buffer_view index_buffer_view;
	};
	std::vector<primitive> primitives;

	void render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world);

	struct texture_info
	{
		int index = -1; // required.
		int texcoord = 0; // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
	};
	// https://www.khronos.org/registry/glTF/specs/2.0/glTF-2.0.html#reference-material-normaltextureinfo
	struct normal_texture_info
	{
		int index = -1;  // required
		int texcoord = 0;    // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
		float scale = 1;    // scaledNormal = normalize((<sampled normal texture value> * 2.0 - 1.0) * vec3(<normal scale>, <normal scale>, 1.0))
	};
	// https://www.khronos.org/registry/glTF/specs/2.0/glTF-2.0.html#reference-material-occlusiontextureinfo
	struct occlusion_texture_info
	{
		int index = -1;   // required
		int texcoord = 0;     // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
		float strength = 1;  // A scalar parameter controlling the amount of occlusion applied. A value of `0.0` means no occlusion. A value of `1.0` means full occlusion. This value affects the final occlusion value as: `1.0 + strength * (<sampled occlusion texture value> - 1.0)`.
	};
	// https://www.khronos.org/registry/glTF/specs/2.0/glTF-2.0.html#reference-material-pbrmetallicroughness
	struct pbr_metallic_roughness
	{
		float basecolor_factor[4] = { 1, 1, 1, 1 };  // len = 4. default [1,1,1,1]
		texture_info basecolor_texture;
		float metallic_factor = 1;   // default 1
		float roughness_factor = 1;  // default 1
		texture_info metallic_roughness_texture;
	};
	struct material
	{
		std::string name;
		struct cbuffer
		{
			float emissive_factor[3] = { 0, 0, 0 };  // length 3. default [0, 0, 0]
			int alpha_mode = 0;	// "OPAQUE" : 0, "MASK" : 1, "BLEND" : 2
			float alpha_cutoff = 0.5f; // default 0.5
			int double_sided = 0; // default false;

			pbr_metallic_roughness pbr_metallic_roughness;

			normal_texture_info normal_texture;
			occlusion_texture_info occlusion_texture;
			texture_info emissive_texture;
		};
		cbuffer data;
	};
	std::vector<material> materials;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> material_resource_view;

	struct texture
	{
		std::string name;
		int source{ -1 };
	};
	std::vector<texture> textures;
	struct image
	{
		std::string name;
		int width{ -1 };
		int height{ -1 };
		int component{ -1 };
		int bits{ -1 };			// bit depth per channel. 8(byte), 16 or 32.
		int pixel_type{ -1 };	// pixel type(TINYGLTF_COMPONENT_TYPE_***). usually UBYTE(bits = 8) or USHORT(bits = 16)
		int buffer_view;		// (required if no uri)
		std::string mime_type;	// (required if no uri) ["image/jpeg", "image/png", "image/bmp", "image/gif"]
		std::string uri;		// (required if no mimeType) uri is not decoded(e.g. whitespace may be represented as %20)

		// When this flag is true, data is stored to `image` in as-is format(e.g. jpeg
		// compressed for "image/jpeg" mime) This feature is good if you use custom
		// image loader function. (e.g. delayed decoding of images for faster glTF
		// parsing) Default parser for Image does not provide as-is loading feature at
		// the moment. (You can manipulate this by providing your own LoadImageData
		// function)
		bool as_is{ false };
	};
	std::vector<image> images;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> texture_resource_views;

private:
	void fetch_nodes(const tinygltf::Model& gltf_model);
	void cumulate_transforms(std::vector<node>& nodes);
	buffer_view make_buffer_view(const tinygltf::Accessor& accessor);
	void fetch_meshes(ID3D11Device* device, const tinygltf::Model& gltf_model);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	struct primitive_constants
	{
		DirectX::XMFLOAT4X4 world;
		int material{ -1 };
		int has_tangent{ 0 };
		int pad[2];
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> primitive_cbuffer;

	void fetch_materials(ID3D11Device* device, const tinygltf::Model& gltf_model);
	void fetch_textures(ID3D11Device* device, const tinygltf::Model& gltf_model);

};

