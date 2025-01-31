#pragma once

#include "Component.h"
#include "../Utilities/JsonHelpers.h"
#include "../Assets/Objects/ModelAsset.h"
#include "../Assets/Objects/MaterialAsset.h"
#include "../Rendering/Mesh.h"
#include <vector>
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
		Material* GetMaterial() const;

		void SetMaterials(const std::vector<Material*>& materials);
		const std::vector<Material*>& GetMaterials() const;

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
			JSON_MAP_TO_ASSET_REFS_SETTER_OPTIONAL (materials, SetMaterialsFromAssets, MaterialAsset)
			JSON_MAP_TO_ASSET_REF_SETTER_OPTIONAL  (model,     SetMeshFromAsset,       ModelAsset)
			JSON_MAP_TO_FACTORY_SETTER_OPTIONAL    (mesh,      SetMesh,                Mesh)

			// Custom deserialization logic to parse the "primitive" field.
			// TODO: Make it a struct and use the JSON_SCHEMA_BEGIN macro
			CUSTOM_JSON_DESERIALIZATION(OnDeserialization)
		JSON_SCHEMA_END()
	#endif

		static void OnDeserialization(const nlohmann::json& in, MeshRenderer& out);

	private:
		void SetMeshFromAsset(ModelAsset* modelAsset);
		void SetMaterialsFromAssets(const std::vector<MaterialAsset*>& materialAssets);

		Mesh* _mesh;
		std::vector<Material*> _materials;
	};
}
