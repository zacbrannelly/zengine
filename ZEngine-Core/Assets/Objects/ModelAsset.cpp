#include "ModelAsset.h"
#include "../../Rendering/Mesh.h"
#include "../../Rendering/Texture2D.h"
#include "../../Rendering/Material.h"
#include "ShaderAsset.h"
#include "../AssetManager.h"
#include "TextureAsset.h"
#include <iostream>

#include <glm\glm.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

using namespace Assimp;

ModelAsset::ModelAsset(std::string name) : Asset(name, ObjectType::MODEL_ASSET)
{
}

Asset* ModelAsset::CreateInstance(std::string name)
{
	return new ModelAsset(name);
}

bool ModelAsset::Load(std::string path)
{
	const aiScene* scene = _importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder);

	if (scene == nullptr)
	{
		std::cout << "MODEL_ASSET: Failed to load model '" << path << "' since assimp returned a null scene!" << std::endl;
 		return false;
	}

	_directory = path.substr(0, path.find_last_of('/'));

	if (!scene->HasMeshes())
	{
		std::cout << "MODEL_ASSET: Faild to import model as it has no mesh data!" << std::endl;
		return false;
	}
	else
	{
		_mesh = LoadMesh(scene);
	}

	if (scene->HasMaterials())
	{
		_materials = LoadMaterials(scene);
	}

	return true;
}

Mesh* ModelAsset::GetMesh() const
{
	return _mesh;
}

const std::vector<Material*> ModelAsset::GetMaterials()
{
	return _materials;
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
	std::vector<Material*> materials;

	for (int i = 1; i < scene->mNumMaterials; i++)
	{
		aiMaterial* sceneMaterial = scene->mMaterials[i];

		aiString name;
		aiGetMaterialString(sceneMaterial, AI_MATKEY_NAME, &name);

		Material* material = new Material(name.C_Str());
		
		// Set the shader to the pbr shader
		material->SetShader(AssetManager::GetInstance()->GetAsset("pbr_direct")->Cast<ShaderAsset>()->GetShader());

		auto diffuseTextures = LoadTextures(sceneMaterial, aiTextureType_DIFFUSE);
		auto specTextures = LoadTextures(sceneMaterial, aiTextureType_SPECULAR);
		auto normalTextures = LoadTextures(sceneMaterial, aiTextureType_NORMALS);

		if (diffuseTextures.size() > 0)
		{
			material->RegisterSampler("albedoTexture");
			material->SetTexture("albedoTexture", diffuseTextures.front()->GetHandle());
		}

		if (specTextures.size() > 0)
		{

		}

		if (normalTextures.size() > 0)
		{
			material->RegisterSampler("normalTexture");
			material->SetTexture("normalTexture", normalTextures.front()->GetHandle());
		}

		// Material light textures
		material->RegisterSampler("roughnessTexture");
		material->RegisterSampler("metallicTexture");
		material->RegisterSampler("aoTexture");

		// Material light uniforms
		material->RegisterUniform("albedoTint", bgfx::UniformType::Vec4, 1);
		material->RegisterUniform("roughness", bgfx::UniformType::Vec4, 1);
		material->RegisterUniform("metallic", bgfx::UniformType::Vec4, 1);
		material->RegisterUniform("ao", bgfx::UniformType::Vec4, 1);

		// Light positions and colours
		material->RegisterUniform("lightPositions", bgfx::UniformType::Vec4, 4);
		material->RegisterUniform("lightColors", bgfx::UniformType::Vec4, 4);

		// World position of the camera
		material->RegisterUniform("camPos", bgfx::UniformType::Vec4, 1);

		auto assetManager = AssetManager::GetInstance();

		auto roughnessAsset = assetManager->LoadAsset("test texture 2", "blank_ao.png", ObjectType::TEXTURE_ASSET)->Cast<TextureAsset>();
		auto metallicAsset = assetManager->LoadAsset("test texture 3", "blank_ao.png", ObjectType::TEXTURE_ASSET)->Cast<TextureAsset>();
		auto aoAsset = assetManager->LoadAsset("test texture 5", "blank_ao.png", ObjectType::TEXTURE_ASSET)->Cast<TextureAsset>();

		// Load the textures into the GPU
		roughnessAsset->LoadTexture();
		metallicAsset->LoadTexture();
		aoAsset->LoadTexture();

		// Set the textures to the shader
		material->SetTexture("roughnessTexture", roughnessAsset->GetTexture()->GetHandle());
		material->SetTexture("metallicTexture", metallicAsset->GetTexture()->GetHandle());
		material->SetTexture("aoTexture", aoAsset->GetTexture()->GetHandle());

		// Set the other material properties
		material->SetUniform("albedoTint", new glm::vec4(1.0f), 1);
		material->SetUniform("roughness", new glm::vec4(0.11f), 1);
		material->SetUniform("metallic", new glm::vec4(0.5f), 1);
		material->SetUniform("ao", new glm::vec4(1.0f), 1);

		auto lightPos = new std::vector<glm::vec4>
		{
			{ -10, -10, -10, 1 },
			{ 10,  10, -10, 1 },
			{ -10,  10, -10, 1 },
			{ 10, -10, -10, 1 }
		};

		auto lightColors = new std::vector<glm::vec4>
		{
			{ 300, 300, 300, 0 },
			{ 300, 300, 300, 0 },
			{ 300, 300, 300, 0 },
			{ 300, 300, 300, 0 },
		};

		// Light properties & camera position
		material->SetUniform("lightPositions", &(*lightPos)[0], lightPos->size());
		material->SetUniform("lightColors", &(*lightColors)[0], lightColors->size());
		material->SetUniform("camPos", new glm::vec4(0, 0, -10, 1), 1);

		materials.push_back(material);
	}

	return materials;
}

std::vector<Texture2D*> ModelAsset::LoadTextures(const aiMaterial* mat, aiTextureType type)
{
	auto assetManager = AssetManager::GetInstance();
	std::vector<Texture2D*> textures;

	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);

		// Attemp to load the texture
		auto newTexture = assetManager->LoadAsset(GetName() + "_" + path.C_Str(), _directory + '/' + path.C_Str(), ObjectType::TEXTURE_ASSET);

		if (newTexture != nullptr)
		{
			auto textureAsset = newTexture->Cast<TextureAsset>();

			if (!textureAsset->IsLoaded())
			{
				if (textureAsset->LoadTexture())
				{
					textures.push_back(textureAsset->GetTexture());
				}

				// TODO: Release asset on failure to load POSSBILE MEMORY LEAK
			}
			else
				textures.push_back(textureAsset->GetTexture());
		}
	}

	return textures;
}

ModelAsset::~ModelAsset()
{
}
