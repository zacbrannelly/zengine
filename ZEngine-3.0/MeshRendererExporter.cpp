#include "MeshRendererExporter.h"
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/ModelAsset.h>
#include <ZEngine-Core/Assets/Objects/MaterialAsset.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/Rendering/Mesh.h>
#include <ZEngine-Core/Component/MeshRenderer.h>

using namespace nlohmann;

json MeshRendererExporter::ExportImpl(ZObject* obj)
{
	auto meshRenderer = static_cast<MeshRenderer*>(obj);
	auto catalog = AssetManager::GetInstance()->GetCatalog();

	json result;

	result["name"] = obj->GetName();
	result["type"] = obj->GetType();

	auto& materials = result["materials"] = json::array_t();

	for (const auto& material : meshRenderer->GetMaterials())
	{
		auto asset = GetMaterialAsset(material);
		auto materialId = catalog != nullptr ? catalog->GetAssetIDFromPath(asset->GetPath()) : -1;

		if (materialId >= 0)
			materials.push_back(materialId);
	}

	auto modelAsset = GetModelAsset(meshRenderer->GetMesh());

	if (modelAsset != nullptr)
	{
		auto modelId = catalog != nullptr ? catalog->GetAssetIDFromPath(modelAsset->GetPath()) : -1;

		if (modelId >= 0)
			result["model"] = modelId;
	}
	else
	{
		auto mesh = meshRenderer->GetMesh();
		auto& meshObj = result["mesh"] = json::object_t();

		auto& vertices = result["vertices"] = json::array_t(mesh->GetVertices().size(), { 0, 0, 0 });
		memcpy(&vertices[0], mesh->GetVertices().data(), mesh->GetVertices().size() * sizeof(glm::vec3));

		auto& colors = result["colors"] = json::array_t(mesh->GetColors().size(), { 0, 0, 0, 1 });
		memcpy(&colors[0], mesh->GetColors().data(), mesh->GetColors().size() * sizeof(glm::vec4));

		auto& texCoords = result["texCoords"] = json::array_t(mesh->GetTextureCoords().size(), { 0, 0 });
		memcpy(&texCoords[0], mesh->GetTextureCoords().data(), mesh->GetTextureCoords().size() * sizeof(glm::vec2));

		auto& normals = result["normals"] = json::array_t(mesh->GetNormals().size(), { 0, 0, 0 });
		memcpy(&normals[0], mesh->GetNormals().data(), mesh->GetNormals().size() * sizeof(glm::vec3));

		auto& subMeshes = result["indices"] = json::array_t();

		for (int i = 0; i < mesh->GetSubMeshCount(); i++)
		{
			const auto& indices = mesh->GetIndices(i);
			auto jsonIndices = json::array_t(indices.size(), 0);

			memcpy(jsonIndices.data(), indices.data(), indices.size() * sizeof(uint32_t));
			subMeshes.push_back(jsonIndices);
		}
	}

	return result;
}

MaterialAsset* MeshRendererExporter::GetMaterialAsset(Material* mat) const
{
	auto assetManager = AssetManager::GetInstance();

	for (auto& asset : assetManager->GetAssets())
	{
		if (asset->GetType() == MATERIAL_ASSET)
		{
			auto materialAsset = asset->Cast<MaterialAsset>();

			if (materialAsset->GetMaterial() == mat)
			{
				return materialAsset;
			}
		}
	}

	return nullptr;
}

ModelAsset* MeshRendererExporter::GetModelAsset(Mesh* mesh) const
{
	auto assetManager = AssetManager::GetInstance();

	for (auto& asset : assetManager->GetAssets())
	{
		if (asset->GetType() == MODEL_ASSET)
		{
			auto modelAsset = asset->Cast<ModelAsset>();

			if (modelAsset->GetMesh() == mesh)
			{
				return modelAsset;
			}
		}
	}

	return nullptr;
}