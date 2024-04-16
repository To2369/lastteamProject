#include "gltf_model_static_batching.h"
#include <stack>
#include <functional>
#include <filesystem>

#include "misc.h"
#include "Graphics/shader.h"
#include "texture.h"

bool null_load_image_data(tinygltf::Image*, const int, std::string*, std::string*, int, int, const unsigned char*, int, void*);

gltf_model_static_batching::gltf_model_static_batching(ID3D11Device* device, const std::string& filename) : filename(filename)
{
	tinygltf::TinyGLTF tiny_gltf;
	tiny_gltf.SetImageLoader(null_load_image_data, nullptr);

	tinygltf::Model gltf_model;
	std::string error, warning;
	bool succeeded{ false };
	if (filename.find(".glb") != std::string::npos)
	{
		succeeded = tiny_gltf.LoadBinaryFromFile(&gltf_model, &error, &warning, filename.c_str());
	}
	else if (filename.find(".gltf") != std::string::npos)
	{
		succeeded = tiny_gltf.LoadASCIIFromFile(&gltf_model, &error, &warning, filename.c_str());
	}

	_ASSERT_EXPR_A(warning.empty(), warning.c_str());
	_ASSERT_EXPR_A(error.empty(), error.c_str());
	_ASSERT_EXPR_A(succeeded, L"Failed to load glTF file");

}