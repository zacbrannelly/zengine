#include "MeshFactory.h"
#include "Mesh.h"
#include "../Misc/Factory.h"

Mesh* MeshFactory::CreateCube(std::string name)
{
	std::vector<glm::vec3> verts
	{
		// Front face
		{-1.0f, -1.0f, -1.0f }, // top left 6
		{ 1.0f, -1.0f, -1.0f }, // top right 7
		{-1.0f,  1.0f, -1.0f }, // bottom left 4
		{ 1.0f, -1.0f, -1.0f }, // top right 7
		{ 1.0f,  1.0f, -1.0f }, // bottom right 5 
		{-1.0f,  1.0f, -1.0f }, // bottom left 4

		// Back face
		{ 1.0f, -1.0f,  1.0f }, // top right 3
		{-1.0f, -1.0f,  1.0f }, // top left 2
		{ 1.0f,  1.0f,  1.0f }, // bototm right 1
		{-1.0f, -1.0f,  1.0f }, // top left 2
		{-1.0f,  1.0f,  1.0f }, // bottom left 0
		{ 1.0f,  1.0f,  1.0f }, // bototm right 1

		// Left face
		{-1.0f, -1.0f,  1.0f }, // top left 2
		{-1.0f, -1.0f, -1.0f }, // top left 6
		{-1.0f,  1.0f,  1.0f }, // bottom left 0
		{-1.0f, -1.0f, -1.0f }, // top left 6
		{-1.0f,  1.0f, -1.0f }, // bottom left 4
		{-1.0f,  1.0f,  1.0f }, // bottom left 0

		// Right face
		{ 1.0f, -1.0f, -1.0f }, // top right 7
		{ 1.0f, -1.0f,  1.0f }, // top right 3
		{ 1.0f,  1.0f, -1.0f }, // bottom right 5 
		{ 1.0f, -1.0f,  1.0f }, // top right 3
		{ 1.0f,  1.0f,  1.0f }, // bototm right 1
		{ 1.0f,  1.0f, -1.0f }, // bottom right 5 

		// Top face
		{-1.0f, -1.0f,  1.0f }, // top left 2
		{ 1.0f, -1.0f,  1.0f }, // top right 3
		{-1.0f, -1.0f, -1.0f }, // top left 6
		{ 1.0f, -1.0f,  1.0f }, // top right 3
		{ 1.0f, -1.0f, -1.0f }, // top right 7
		{-1.0f, -1.0f, -1.0f }, // top left 6

		// Bottom face
		{-1.0f,  1.0f, -1.0f }, // bottom left 4
		{ 1.0f,  1.0f, -1.0f }, // bottom right 5 
		{-1.0f,  1.0f,  1.0f }, // bottom left 0
		{ 1.0f,  1.0f, -1.0f }, // bottom right 5 
		{ 1.0f,  1.0f,  1.0f }, // bototm right 1
		{-1.0f,  1.0f,  1.0f }, // bottom left 0
	};

	std::vector<glm::vec2> texCoords
	{
		// Front
		{ 0, 0 }, // top left
		{ 1, 0 }, // top right
		{ 0, 1 }, // bottom left
		{ 1, 0 }, // top right
		{ 1, 1 }, // bottom right
		{ 0, 1 }, // bottom left

		// Back
		{ 0, 0 }, // top left
		{ 1, 0 }, // top right
		{ 0, 1 }, // bottom left
		{ 1, 0 }, // top right
		{ 1, 1 }, // bottom right
		{ 0, 1 }, // bottom left

		// Left
		{ 0, 0 }, // top left
		{ 1, 0 }, // top right
		{ 0, 1 }, // bottom left
		{ 1, 0 }, // top right
		{ 1, 1 }, // bottom right
		{ 0, 1 }, // bottom left

		// Right
		{ 0, 0 }, // top left
		{ 1, 0 }, // top right
		{ 0, 1 }, // bottom left
		{ 1, 0 }, // top right
		{ 1, 1 }, // bottom right
		{ 0, 1 }, // bottom left

		// Top
		{ 0, 0 }, // top left
		{ 1, 0 }, // top right
		{ 0, 1 }, // bottom left
		{ 1, 0 }, // top right
		{ 1, 1 }, // bottom right
		{ 0, 1 }, // bottom left

		// Bottom
		{ 0, 0 }, // top left
		{ 1, 0 }, // top right
		{ 0, 1 }, // bottom left
		{ 1, 0 }, // top right
		{ 1, 1 }, // bottom right
		{ 0, 1 }, // bottom left
	};

	std::vector<glm::vec4> colors(3 * 2 * 6, { 1, 1, 1, 1 });

	std::vector<glm::vec3> normals
	{
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},

		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},

		{-1, 0, 0},
		{-1, 0, 1},
		{-1, 0, 1},
		{-1, 0, 1},
		{-1, 0, 1},
		{-1, 0, 1},

		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},

		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},

		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
	};

	std::vector<uint16_t> indices
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		31, 32, 33, 34, 35
	};

	auto newMesh = Factory::CreateInstance<Mesh>(name, ObjectType::MESH);
	newMesh->SetVertices(verts);
	newMesh->SetColors(colors);
	newMesh->SetIndices(indices);
	newMesh->SetNormals(normals);
	newMesh->SetTextureCoords(texCoords);

	return newMesh;
}

Mesh* MeshFactory::CreateSphere(std::string name, int resolution)
{
	auto newMesh = Factory::CreateInstance<Mesh>(name, ObjectType::MESH);

	std::vector<glm::vec3> vertices;
	std::vector<uint16_t> indices;

	GeneratePlaneVertices(glm::vec3( 0.0f, -0.5f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::BOTTOM);
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.5f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::TOP);
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.0f, -0.5f), resolution, resolution, vertices, indices, PlaneOrientation::FRONT);
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.0f,  0.5f), resolution, resolution, vertices, indices, PlaneOrientation::BACK);
	GeneratePlaneVertices(glm::vec3(-0.5f, 0.0f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::LEFT);
	GeneratePlaneVertices(glm::vec3( 0.5f, 0.0f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::RIGHT);
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.5f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::TOP);

	std::vector<glm::vec3> normals;

	for (auto& vertex : vertices)
	{
		vertex = glm::normalize(vertex);
		normals.push_back(vertex);
	}

	std::vector<glm::vec4> colors(vertices.size(), { 1, 1, 1, 1 });

	newMesh->SetVertices(vertices);
	newMesh->SetColors(colors);
	newMesh->SetIndices(indices);
	newMesh->SetNormals(normals);

	return newMesh;
}

Mesh* MeshFactory::CreateRectangle(std::string name)
{
	std::vector<glm::vec3> verts
	{
		{ -0.5f, -0.5f, 0 }, // top left - 0
		{  0.5f, -0.5f, 0 }, // top right - 1
		{ -0.5f,  0.5f, 0 }, // bottom left - 2
		{  0.5f,  0.5f, 0 }, // bottom right - 3
	};

	std::vector<glm::vec2> texCoords
	{
		{ 0, 0 }, 
		{ 1, 0 }, 
		{ 0, 1 }, 
		{ 1, 1 }
	};

	std::vector<glm::vec4> colors(6, { 1, 1, 1, 1 });

	std::vector<uint16_t> indices
	{
		0, 3, 2,
		0, 1, 3
	};

	auto newMesh = Factory::CreateInstance<Mesh>(name, ObjectType::MESH);
	newMesh->SetVertices(verts);
	newMesh->SetColors(colors);
	newMesh->SetIndices(indices);
	newMesh->SetTextureCoords(texCoords);

	return newMesh;
}

Mesh* MeshFactory::CreatePlane(std::string name, int width, int height, PlaneOrientation facing)
{
	std::vector<glm::vec3> verts;
	std::vector<uint16_t> indices;

	GeneratePlaneVertices(glm::vec3(0, 0, 0), width, height, verts, indices, facing);

	std::vector<glm::vec4> colors(verts.size(), { 1, 1, 1, 1 });

	auto newMesh = Factory::CreateInstance<Mesh>(name, ObjectType::MESH);
	newMesh->SetVertices(verts);
	newMesh->SetColors(colors);
	newMesh->SetIndices(indices);

	return newMesh;
}

void MeshFactory::GeneratePlaneVertices(const glm::vec3& pos, int width, int height, std::vector<glm::vec3>& vertices, std::vector<uint16_t>& indices, PlaneOrientation facing)
{
	float xx = -0.5f;
	float yy = -0.5f;

	float stepSizeX = 1.0f / (width - 1);
	float stepSizeY = 1.0f / (height - 1);

	int prevSize = vertices.size();

	for (int y = 0; y < height; ++y)
	{
		xx = -0.5f;

		for (int x = 0; x < width; ++x)
		{
			switch (facing)
			{
			case PlaneOrientation::FRONT:
				vertices.push_back({ xx, yy, 0 });
				break;

			case PlaneOrientation::BACK:
				vertices.push_back({ -xx, yy, 0 });
				break;

			case PlaneOrientation::TOP:
				vertices.push_back({ xx, 0, yy });
				break;

			case PlaneOrientation::BOTTOM:
				vertices.push_back({ xx, 0, -yy });
				break;

			case PlaneOrientation::LEFT:
				vertices.push_back({ 0, yy, -xx });
				break;

			case PlaneOrientation::RIGHT:
				vertices.push_back({ 0, yy, xx });
				break;
			}

			vertices[vertices.size() - 1] += pos;

			xx += stepSizeX;
		}

		yy += stepSizeY;
	}


	int startIndex = prevSize;
	int index = 0;
	for (int index = 0; index < vertices.size(); index++)
	{
		if ((index + 1) % width == 0)
		{
			continue;
		}

		if (index / width == height - 1)
		{
			break;
		}

		indices.push_back(startIndex + index + 1);
		indices.push_back(startIndex + index + width);
		indices.push_back(startIndex + index);

		indices.push_back(startIndex + index + 1);
		indices.push_back(startIndex + index + width + 1);
		indices.push_back(startIndex + index + width);
	}
}
