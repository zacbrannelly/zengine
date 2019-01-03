#include "MeshRendererImporter.h"
#include "../MeshRenderer.h"
#include "../../Misc/Factory.h"
#include "../../Assets/AssetManager.h"
#include "../../Assets/AssetCatalog.h"
#include "../../Assets/Objects/MaterialAsset.h"
#include "../../Assets/Objects/ModelAsset.h"
#include "../../Rendering/Mesh.h"
#include "../../Rendering/MeshFactory.h"

#include "../../Map/Objects/Entity.h"

using namespace std;
using namespace nlohmann;

ZObject* MeshRendererImporter::ImportImpl(string name, json::object_t& values)
{
	auto meshRenderer = Factory::CreateInstance<MeshRenderer>(name, MESH_RENDERER);
	auto assetManager = AssetManager::GetInstance();

	// TODO: Be able to load per-instance material properties
	if (HasKey("materials", values))
	{
		vector<Material*> materials;

		// JSON should contain material id's that are recorded in the AssetCatalog
		auto materialIds = ReadDynamicArray<int>("materials", values);
		for (auto materialId : materialIds)
		{
			string path;
			ObjectType type;
			if (assetManager->GetCatalog()->GetAssetPathFromID(materialId, path, type) && type == MATERIAL_ASSET)
			{
				auto matAsset = assetManager->FindAssetFromPath(path);

				if (matAsset == nullptr)
					matAsset = assetManager->LoadAsset(path, path, type);

				if (matAsset != nullptr)
				{
					materials.push_back(matAsset->Cast<MaterialAsset>()->GetMaterial());
				}
			}	
		}

		// Only set the materials if any were loaded
		if (materials.size() > 0)
			meshRenderer->SetMaterials(materials);
	}

	if (HasKey("mesh", values))
	{
		Mesh* testMesh = nullptr;

		auto meshObj = values.at("mesh").get<json::object_t>();

		// Required fields
		auto vertices = ReadDynamicArray<float>("vertices", meshObj);
		auto subMeshes = ReadDynamicArray<json::object_t>("subMesh", meshObj);

		// Optional fields
		auto colors = ReadDynamicArray<float>("colors", meshObj);
		auto texCoords = ReadDynamicArray<float>("texCoords", meshObj);
		auto normals = ReadDynamicArray<float>("normals", meshObj);
		
		// Convert vertices from float[] to vec3[]
		vector<glm::vec3> vertexData(vertices.size() / 3, { 0, 0, 0 });
		memcpy(vertexData.data(), vertices.data(), sizeof(float) * vertices.size());

		auto mesh = Factory::CreateInstance<Mesh>(name + "_mesh", MESH);
		mesh->SetVertices(vertexData);

		for (int i = 0; i < subMeshes.size(); i++)
		{
			auto& subMesh = subMeshes[i];
			const auto& indices = subMesh["indices"];
			const auto& mode = subMesh["mode"];

			// Convert the array of json int into vector of uint32
			auto data = vector<uint32_t>();
			for (const auto& index : indices)
				data.push_back(index.get<uint32_t>());

			mesh->SetIndices(i, data);
			mesh->SetMode(i, (DrawMode)mode.get<uint32_t>());
		}

		if (colors.size() > 0)
		{
			// Convert colors from float[] to vec3[]
			vector<glm::vec4> colorData(colors.size() / 4, { 0, 0, 0, 1 });
			memcpy(colorData.data(), colors.data(), sizeof(float) * colors.size());
			mesh->SetColors(colorData);
		}

		if (normals.size() > 0)
		{
			// Convert normals from float[] to vec3[]
			vector<glm::vec3> normalData(normals.size() / 3, { 0, 0, 0 });
			memcpy(normalData.data(), normals.data(), sizeof(float) * normals.size());
			mesh->SetNormals(normalData);
		}

		if (texCoords.size() > 0)
		{
			// Convert texCoords from float[] to vec3[]
			vector<glm::vec2> texCoordData(texCoords.size() / 2, { 0, 0 });
			memcpy(texCoordData.data(), texCoords.data(), sizeof(float) * texCoords.size());
			mesh->SetTextureCoords(texCoordData);
		}

		meshRenderer->SetMesh(mesh);
	}

	if (HasKey("primitive", values))
	{
		string primitiveType = ReadString("primitive", values);

		if (primitiveType == "cube")
		{
			meshRenderer->SetMesh(MeshFactory::CreateCube(name + "_mesh"));
		}
		else if (primitiveType == "sphere")
		{
			meshRenderer->SetMesh(MeshFactory::CreateSphereStrip(name + "_mesh"));
		}
		else if (primitiveType == "rectangle")
		{
			meshRenderer->SetMesh(MeshFactory::CreateRectangle(name + "_mesh"));
		}
		else if (primitiveType == "plane")
		{
			int width = 10;
			int height = 10;
			PlaneOrientation facing = FRONT;

			if (HasKey("plane_size", values))
			{
				auto data = ReadArray<int, 2>("plane_size", values);
				width = data[0];
				height = data[1];
			}

			if (HasKey("plane_facing", values))
			{
				facing = (PlaneOrientation)ReadUnsignedInt("plane_facing", values);
			}

			meshRenderer->SetMesh(MeshFactory::CreatePlane(name + "_mesh", width, height, facing));
		}
		
	}

	if (HasKey("model", values))
	{
		int modelId = ReadInt("model", values);

		string path;
		ObjectType type;
		if (assetManager->GetCatalog()->GetAssetPathFromID(modelId, path, type))
		{
			auto modelAsset = assetManager->FindAssetFromPath(path);

			if (modelAsset == nullptr)
				modelAsset = assetManager->LoadAsset(path, path, type);

			if (modelAsset != nullptr)
			{
				meshRenderer->SetMesh(modelAsset->Cast<ModelAsset>()->GetMesh());

				// TODO: Set materials from the model (still too buggy)
			}
		}
	}

	return meshRenderer;
}