#pragma once

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <vector>
#include <map>
#include "../Asset.h"

class Texture2D;
class Material;
class Mesh;

class ModelAsset : public Asset
{
public:
	ModelAsset(std::string name);
	~ModelAsset();

	bool Load(std::string path) override;
	void Release() override;

	Mesh* GetMesh() const;
	const std::vector<Material*> GetMaterials();

	static Asset* CreateInstance(std::string name);
	
	static ObjectType GetStaticType()
	{
		return MODEL_ASSET;
	}
private:

	Mesh* LoadMesh(const aiScene* scene);
	std::vector<Texture2D*> LoadTextures(const aiMaterial* material, aiTextureType type);
	std::vector<Material*>  LoadMaterials(const aiScene* scene);

	Assimp::Importer _importer;
	Mesh* _mesh;
	std::vector<Material*> _materials;
	std::string _directory;
};

