#pragma once

#include "Component.h"
#include "../Utilities/JsonHelpers.h"
#include "../Assets/Objects/ModelAsset.h"
#include "../Assets/Objects/MaterialAsset.h"
#include "../Rendering/Mesh.h"
#include <vector>
#include <map>
#include <uuid.h>

namespace ZEngine
{
	class Mesh;
	class Material;
	class Transform;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		void SetMesh(Mesh* mesh);
		Mesh* GetMesh() const;

		void SetMaterial(Material* material);
		void SetMaterial(Material* material, int index);
		void SetMaterialAsset(MaterialAsset* materialAsset, int index);
		Material* GetMaterial() const;

		void SetMaterials(const std::vector<Material*>& materials);
		const std::vector<Material*>& GetMaterials() const;

		void SetModelAsset(ModelAsset* modelAsset);
		ModelAsset* GetModelAsset() const;

		void SetMaterialAssets(const std::vector<MaterialAsset*>& materialAssets);
		const std::vector<MaterialAsset*>& GetMaterialAssets() const;

		void SetFromPrimitiveShape(const std::string& primitive, const nlohmann::json& metadata);

		virtual void Init();
		virtual void Update();
		virtual void Render(int viewId);
		virtual void Render(int viewId, Transform* transform);

		static ZObject* CreateInstance(std::string name, ObjectType type);
		static ZObject* Copy(std::string name, ZObject* object);

		static ObjectType GetStaticType()
		{
			return MESH_RENDERER;
		}

	#ifndef SWIG
		// Allow serialization / deserialization
		JSON_SCHEMA_BEGIN(MeshRenderer)
			INCLUDE_ASSET_REFERENCES()
			JSON_MAP_TO_ASSET_REFS_GETTER_SETTER_OPTIONAL (materials, GetMaterialAssets, SetMaterialAssets, MaterialAsset)
			JSON_MAP_TO_ASSET_REF_GETTER_SETTER_OPTIONAL  (model,     GetModelAsset,     SetModelAsset,     ModelAsset)
			JSON_MAP_TO_FACTORY_GETTER_SETTER_OPTIONAL    (mesh,      GetSavableMesh,    SetMesh,           Mesh)

			// Custom deserialization logic to parse the "primitive" & "uniforms" fields
			CUSTOM_JSON_DESERIALIZATION(OnDeserialization)
			CUSTOM_JSON_SERIALIZATION(OnSerialization)
		JSON_SCHEMA_END()
	#endif

		static void OnDeserialization(const nlohmann::json& in, MeshRenderer& out);
		static void OnSerialization(nlohmann::json& out, const MeshRenderer& in);

	private:
		// Returns the mesh if it is not a model or primitive shape
		Mesh* GetSavableMesh() const;

		Mesh* _mesh;
		std::vector<Material*> _materials;

		ModelAsset* _modelAsset;
		std::vector<MaterialAsset*> _materialAssets;
		std::map<Material*, MaterialAsset*> _materialAssetMap;

		std::string _primitiveShape;
	};
}
