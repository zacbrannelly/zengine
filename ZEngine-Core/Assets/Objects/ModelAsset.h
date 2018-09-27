#pragma once

#include <assimp\Importer.hpp>
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

	Mesh* GetMesh() const;

private:

	Mesh* LoadMesh(const aiScene* scene);
	std::vector<Texture2D*> LoadTextures(const aiScene* scene);
	std::vector<Material*>  LoadMaterials(const aiScene* scene);

	Assimp::Importer _importer;
	Mesh* _mesh;
};

