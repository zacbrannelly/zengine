#pragma once

#include "../Asset.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace ZEngine
{
	class Material;

	class MaterialAsset : public Asset
	{
	public:
		MaterialAsset(std::string name);
		~MaterialAsset();

		bool Load(std::string path) override;
		void Release() override;
		Material* GetMaterial() const;

		static ZObject* CreateInstance(std::string name, ObjectType type);
		static ZObject* CreateDefaultInstance(std::string name, ObjectType type);

		static ObjectType GetStaticType()
		{
			return MATERIAL_ASSET;
		}

	private:
		void ReadTextures(nlohmann::json::array_t& values, Material* material);
		void ReadUniforms(nlohmann::json::array_t& values, Material* material);
		void ReadShader(nlohmann::json& identifier, Material* material);

		Material* _material;
	};
}
