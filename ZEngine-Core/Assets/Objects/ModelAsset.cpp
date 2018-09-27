#include "ModelAsset.h"
#include "../../Rendering/Mesh.h"
#include "../../Rendering/Texture2D.h"
#include "../AssetManager.h"
#include "TextureAsset.h"
#include <iostream>

#include <glm\glm.hpp>
#include <assimp\scene.h>
#include <assimp\material.h>
#include <assimp\postprocess.h>

using namespace Assimp;

ModelAsset::ModelAsset(std::string name) : Asset(name, ObjectType::MODEL_ASSET)
{
}

bool ModelAsset::Load(std::string path)
{
	const aiScene* scene = _importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder);

	if (scene == nullptr)
	{
		std::cout << "MODEL_ASSET: Failed to load model '" << path << "' since assimp returned a null scene!" << std::endl;
 		return false;
	}

	Mesh* mesh = nullptr;

	if (!scene->HasMeshes())
	{
		std::cout << "MODEL_ASSET: Faild to import model as it has no mesh data!" << std::endl;
		return false;
	}
	else
	{
		_mesh = LoadMesh(scene);
	}

	if (scene->HasTextures())
	{
		// TODO: Load textyres
	}

	if (scene->HasMaterials())
	{
		// TODO: Load materials
	}

	return true;
}

Mesh* ModelAsset::GetMesh() const
{
	return _mesh;
}

Mesh* ModelAsset::LoadMesh(const aiScene* scene)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::map<int, std::vector<uint32_t>> subMeshes;

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		int offset = vertices.size();

		for (int j = 0; j < mesh->mNumVertices; j++)
		{
			auto vert = mesh->mVertices[j];
			vertices.push_back({ vert.x, vert.y, vert.z });

			if (mesh->HasNormals())
			{
				auto normal = mesh->mNormals[j];
				normals.push_back({ normal.x, normal.y, normal.z });
			}

			if (mesh->HasTextureCoords(0))
			{
				auto texCoord = mesh->mTextureCoords[0][j];		
				texCoords.push_back({ texCoord.x, texCoord.y });
			}

			if (mesh->HasVertexColors(0))
			{
				auto color = mesh->mColors[0][j];
				colors.push_back({ color.r, color.g, color.b, color.a });
			}
		}

		auto& indices = subMeshes[mesh->mMaterialIndex];

		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			auto face = mesh->mFaces[j];

			for (int k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(offset + face.mIndices[k]);
			}
		}
	}

	auto mesh = new Mesh(GetName());

	mesh->SetVertices(vertices);
	mesh->SetNormals(normals);
	mesh->SetTextureCoords(texCoords);

	// If no colour, set the default to white
	if (colors.size() > 0)
		mesh->SetColors(colors);
	else
		mesh->SetColors(std::vector<glm::vec4>(vertices.size(), { 1, 1, 1, 1 }));


	for (auto pair : subMeshes)
	{
		mesh->SetIndices(pair.first, pair.second);
	}

	return mesh;
}

std::vector<Material*> ModelAsset::LoadMaterials(const aiScene* scene)
{

	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		aiColor4D diffuse;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

		// TODO: Not done here
	}

	return std::vector<Material*>();
}

std::vector<Texture2D*> ModelAsset::LoadTextures(const aiScene* scene)
{
	auto assetManager = AssetManager::GetInstance();
	std::vector<Texture2D*> textures;

	for (int i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* texture = scene->mTextures[i];

		// Attemp to load the texture
		auto newTexture = assetManager->LoadAsset(GetName() + "_" + texture->mFilename.C_Str(), texture->mFilename.C_Str(), ObjectType::TEXTURE_ASSET);

		if (newTexture != nullptr)
		{
			auto textureAsset = newTexture->Cast<TextureAsset>();
			if (textureAsset->LoadTexture())
			{
				textures.push_back(textureAsset->GetTexture());
			}
		}
	}

	return textures;
}

ModelAsset::~ModelAsset()
{
}
