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

		auto& vertices = meshObj["vertices"] = json::array_t();
		for (const auto& vert : mesh->GetVertices())
		{
			vertices.push_back(vert.x);
			vertices.push_back(vert.y);
			vertices.push_back(vert.z);
		}

		auto& colors = meshObj["colors"] = json::array_t();
		for (const auto& color : mesh->GetColors())
		{
			colors.push_back(color.r);
			colors.push_back(color.g);
			colors.push_back(color.b);
			colors.push_back(color.a);
		}

		auto& texCoords = meshObj["texCoords"] = json::array_t();
		for (const auto& texCoord : mesh->GetTextureCoords())
		{
			texCoords.push_back(texCoord.x);
			texCoords.push_back(texCoord.y);
		}

		auto& normals = meshObj["normals"] = json::array_t();
		for (const auto& normal : mesh->GetNormals())
		{
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
		}

		auto& subMeshes = meshObj["subMesh"] = json::array_t();

		for (int i = 0; i < mesh->GetSubMeshCount(); i++)
		{
			const auto& indices = mesh->GetIndices(i);
			auto subMeshObj = json::object_t();

			subMeshObj["mode"] = (uint32_t)mesh->GetMode(i);
			auto& jsonIndices = subMeshObj["indices"] = json::array_t();

			for (const auto& i : indices)
			{
				jsonIndices.push_back(i);
			}
			
			subMeshes.push_back(subMeshObj);
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