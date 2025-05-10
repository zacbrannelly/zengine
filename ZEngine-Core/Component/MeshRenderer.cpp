#include "MeshRenderer.h"
#include "Transform.h"
#include "../Map/Objects/Entity.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/MeshFactory.h"
#include "../Rendering/Material.h"

using namespace std;
using namespace ZEngine;

static const std::string MODULE_NAME = "MeshRenderer";

struct UniformOverride
{
	std::string name;
	float* data { nullptr };
	int numFloats { 0 };
	int numElements { 0 };

	void Release()
	{
		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

	static void OnDeserialization(const nlohmann::json& in, UniformOverride& out)
	{
		if (in.contains("data"))
		{
			auto data = in.at("data");
			if (data.is_array())
			{
				out.data = new float[data.size()];
				for (size_t i = 0; i < data.size(); ++i)
				{
					out.data[i] = data[i].get<float>();
				}
				out.numFloats = static_cast<int>(data.size());
			}
		}
	}

	static void OnSerialization(nlohmann::json& out, const UniformOverride& in)
	{
		if (in.data != nullptr)
		{
			nlohmann::json dataArray = nlohmann::json::array();
			for (int i = 0; i < in.numFloats; ++i)
			{
				dataArray.push_back(in.data[i]);
			}
			out["data"] = dataArray;
		}
	}

	JSON_SCHEMA_BEGIN(UniformOverride)
		JSON_MAP_TO_MEMBER (name, name)
		JSON_MAP_TO_MEMBER (numElements, numElements)
		CUSTOM_JSON_DESERIALIZATION(OnDeserialization)
		CUSTOM_JSON_SERIALIZATION(OnSerialization)
	JSON_SCHEMA_END()
};


MeshRenderer::MeshRenderer() : Component("Mesh Renderer", ObjectType::MESH_RENDERER)
{
	RegisterDerivedType(MESH_RENDERER);
	_mesh = nullptr;
	_modelAsset = nullptr;
}

void MeshRenderer::SetModelAsset(ModelAsset* modelAsset)
{
	_modelAsset = modelAsset;
	_mesh = modelAsset->GetMesh();
}

ModelAsset* MeshRenderer::GetModelAsset() const
{
	return _modelAsset;
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	_modelAsset = nullptr;
	_mesh = mesh;
}

Mesh* MeshRenderer::GetMesh() const
{
	return _mesh;
}

Mesh* MeshRenderer::GetSavableMesh() const
{
	// Only save the mesh in the component JSON if it is not a model or primitive shape
	if (_modelAsset == nullptr && _primitiveShape == "") {
		return _mesh;
	}
	return nullptr;
}

void MeshRenderer::SetMaterial(Material* material)
{
	// Make a new instance of the material that we own.
	auto newMaterial = Factory::Copy<Material>(material->GetName(), material);

	if (_materials.size() == 0)
	{
		_materials.push_back(newMaterial);
	}
	else
	{
		auto existing = _materials[0];
		if (_materials[0] != nullptr) {
			// Remove the old material from the asset list
			if (_materialAssetMap.contains(existing))
			{
				auto range = std::remove(_materialAssets.begin(), _materialAssets.end(), _materialAssetMap[existing]);
				_materialAssets.erase(range, _materialAssets.end());
				_materialAssetMap.erase(existing);
			}

			// We should own any material in the _materials list, so we need to delete the old one.
			delete _materials[0];
			_materials[0] = nullptr;
		}
		_materials[0] = newMaterial;
	}
}

void MeshRenderer::SetMaterial(Material* material, int index)
{
	// Make a new instance of the material that we own.
	auto newMaterial = Factory::Copy<Material>(material->GetName(), material);

	if (index < 0 || index >= _materials.size())
	{
		_materials.push_back(newMaterial);
	}
	else
	{
		auto existing = _materials[index];
		if (existing != nullptr)
		{
			// Remove the old material from the asset list
			if (_materialAssetMap.contains(existing))
			{
				auto range = std::remove(_materialAssets.begin(), _materialAssets.end(), _materialAssetMap[existing]);
				_materialAssets.erase(range, _materialAssets.end());
				_materialAssetMap.erase(existing);
			}

			// We should own any material in the _materials list, so we need to delete the old one.
			delete existing;
			_materials[index] = nullptr;
		}
		_materials[index] = newMaterial;
	}
}

void MeshRenderer::SetMaterialAsset(MaterialAsset* materialAsset, int index)
{
	// Make a new instance of the material that we own.
	auto newMaterial = Factory::Copy<Material>(materialAsset->GetName(), materialAsset->GetMaterial());

	if (index < 0 || index >= _materials.size())
	{
		_materials.push_back(newMaterial);
		_materialAssets.push_back(materialAsset);
		_materialAssetMap[newMaterial] = materialAsset;
	}
	else
	{
		auto existing = _materials[index];
		if (existing != nullptr)
		{
			// Remove the old material from the asset list
			if (_materialAssetMap.contains(existing))
			{
				auto range = std::remove(_materialAssets.begin(), _materialAssets.end(), _materialAssetMap[existing]);
				_materialAssets.erase(range, _materialAssets.end());
				_materialAssetMap.erase(existing);
			}

			// We own any material in the _materials list, so we need to delete the old one.
			delete existing;
			existing = nullptr;
		}

		_materials[index] = newMaterial;
		_materialAssets.push_back(materialAsset);
		_materialAssetMap[newMaterial] = materialAsset;
	}
}

Material* MeshRenderer::GetMaterial() const
{
	return _materials.size() > 0 ? _materials.front() : nullptr;
}

void MeshRenderer::SetMaterials(const vector<Material*>& materials)
{
	// Delete the old materials
	// We should own any material in the _materials list.
	for (auto material : _materials)
	{
		if (material != nullptr)
		{
			delete material;
		}
	}
	_materials.clear();
	_materialAssets.clear();
	_materialAssetMap.clear();

	for (auto material : materials)
	{
		auto newMaterial = Factory::Copy<Material>(material->GetName(), material);
		_materials.push_back(newMaterial);
	}
}

void MeshRenderer::SetMaterialAssets(const vector<MaterialAsset*>& materialAssets)
{
	// Delete the old materials
	// We should own any material in the _materials list.
	for (auto material : _materials)
	{
		if (material != nullptr)
		{
			delete material;
		}
	}

	_materialAssets.clear();
	_materialAssetMap.clear();
	_materials.clear();

	for (auto materialAsset : materialAssets)
	{
		auto newMaterial = Factory::Copy<Material>(materialAsset->GetName(), materialAsset->GetMaterial());
		_materials.push_back(newMaterial);
		_materialAssets.push_back(materialAsset);
		_materialAssetMap[newMaterial] = materialAsset;
	}
}

const vector<MaterialAsset*>& MeshRenderer::GetMaterialAssets() const
{
	return _materialAssets;
}

const vector<Material*>& MeshRenderer::GetMaterials() const
{
	return _materials;
}

void MeshRenderer::SetFromPrimitiveShape(const string& primitive, const nlohmann::json& metadata)
{
	_primitiveShape = primitive;

	if (primitive == "cube")
	{
		SetMesh(MeshFactory::CreateCube("Cube"));
	}
	else if (primitive == "sphere")
	{
		SetMesh(MeshFactory::CreateSphereStrip("Sphere"));
	}
	else if (primitive == "rectangle")
	{
		SetMesh(MeshFactory::CreateRectangle("Rectangle"));
	}
	else if (primitive == "plane")
	{
		int width = 10;
		int height = 10;
		PlaneOrientation facing = FRONT;

		if (metadata.contains("plane_size"))
		{
			auto data = metadata["plane_size"].get<vector<int>>();
			width = data[0];
			height = data[1];
		}

		if (metadata.contains("plane_facing"))
		{
			facing = (PlaneOrientation)metadata["plane_facing"].get<unsigned int>();
		}

		SetMesh(MeshFactory::CreatePlane("Plane", width, height, facing));
	} else {
		throw std::runtime_error("Invalid primitive shape: " + primitive);
	}
}

void MeshRenderer::Init()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::Render(int viewId)
{
	Render(viewId, GetOwner()->GetTransform());
}

void MeshRenderer::Render(int viewId, Transform* transform)
{
	if (_mesh != nullptr)
	{
		auto worldTransform = transform->GetWorldTransformMatrix();
		_mesh->Draw(viewId, _materials, worldTransform);
	}
}

MeshRenderer::~MeshRenderer()
{
}

ZObject* MeshRenderer::CreateInstance(std::string name, ObjectType type)
{
	return new MeshRenderer();
}

ZObject* MeshRenderer::Copy(string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != MESH_RENDERER)
		return nullptr;

	auto source = static_cast<MeshRenderer*>(object);
	auto copy = new MeshRenderer();

	if (copy->_materialAssets.size() > 0)
	{
		copy->SetMaterialAssets(source->GetMaterialAssets());
	}
	else
	{
		copy->SetMaterials(source->GetMaterials());
	}

	if (source->_modelAsset != nullptr)
	{
		copy->SetModelAsset(source->_modelAsset);
	}
	else
	{
		copy->SetMesh(source->_mesh);
	}

	copy->_primitiveShape = source->_primitiveShape;

	return copy;
}

void MeshRenderer::OnDeserialization(const nlohmann::json& in, MeshRenderer& out)
{
	if (in.contains("primitive"))
	{
		auto primitive = in["primitive"].get<std::string>();
		out.SetFromPrimitiveShape(primitive, in);
	}

	if (in.contains("uniforms"))
	{
		auto assetManager = AssetManager::GetInstance();
		auto catalog = assetManager->GetCatalog();

		auto uniformsByMaterial = in["uniforms"].get<nlohmann::json::array_t>();
		for (int i = 0; i < uniformsByMaterial.size(); ++i)
		{
			if (i >= out._materials.size())
			{
				LoggingSystem::GetInstance()->LogError("Material index out of bounds: " + std::to_string(i), MODULE_NAME);
				continue;
			}

			auto& uniforms = uniformsByMaterial[i];

			// Collect a list of overrides for this material.
			std::vector<UniformOverride> overrides;
			for (auto& uniform : uniforms.items())
			{
				auto uniformName = uniform.key();
				auto uniformData = uniform.value();

				UniformOverride override;
				from_json(uniformData, override);
				overrides.push_back(override);
			}

			// Find the material instance based on the location in the JSON.
			Material* materialInstance = out._materials[i];
			if (materialInstance == nullptr)
			{
				LoggingSystem::GetInstance()->LogError("Material instance is null at index: " + std::to_string(i), MODULE_NAME);
				continue;
			}

			// Apply the overrides to the material instance.
			for (auto& override : overrides)
			{
				materialInstance->SetUniform(override.name, override.data, override.numElements);
			}
		}
	}
}

void MeshRenderer::OnSerialization(nlohmann::json& out, const MeshRenderer& in)
{
	if (in._primitiveShape != "")
	{
		out["primitive"] = in._primitiveShape;
	}

	// Serialize the uniforms for each material asset
	out["uniforms"] = nlohmann::json::array();
	for (auto& material : in._materials)
	{
		auto materialObj = nlohmann::json::object();
		for (auto& uniform : material->GetUniforms())
		{
			UniformOverride override;
			override.name = uniform.first;
			override.data = static_cast<float*>(uniform.second.data);
			override.numElements = uniform.second.numElements;

			switch (uniform.second.type)
			{
				case bgfx::UniformType::Vec4:
					override.numFloats = 4 * uniform.second.numElements;
					break;
				case bgfx::UniformType::Mat3:
					override.numFloats = 9 * uniform.second.numElements;
					break;
				case bgfx::UniformType::Mat4:
					override.numFloats = 16 * uniform.second.numElements;
					break;
				default:
					LoggingSystem::GetInstance()->LogError("Unsupported uniform type for serialization: " + uniform.first, MODULE_NAME);
					continue; // Skip unsupported types
			}

			nlohmann::json uniformData;
			to_json(uniformData, override);

			materialObj[uniform.first] = uniformData;
		}
		out["uniforms"].push_back(materialObj);
	}
}
