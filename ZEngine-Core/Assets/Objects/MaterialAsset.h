#pragma once

#include "../Asset.h"
#include <glm/glm.hpp>
#include "../../../json/json.hpp"

class Material;

class MaterialAsset : public Asset
{
public:
	MaterialAsset(std::string name);
	~MaterialAsset();

	bool Load(std::string path) override;
	Material* GetMaterial() const;

	static Asset* CreateInstance(std::string name);

private:
	void ReadTextures(nlohmann::json::array_t& values, Material* material);
	void ReadUniforms(nlohmann::json::array_t& values, Material* material);
	void ReadShader(nlohmann::json::string_t& name, Material* material);

	std::vector<glm::vec4>* ReadVec4From(nlohmann::json::array_t& values, int num);
	std::vector<glm::mat3>* ReadMat3From(nlohmann::json::array_t& values, int num);
	std::vector<glm::mat4>* ReadMat4From(nlohmann::json::array_t& values, int num);

	Material* _material;
};

