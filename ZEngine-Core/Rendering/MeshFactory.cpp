#include "MeshFactory.h"
#include "Mesh.h"
#include "../Misc/Factory.h"

Mesh* MeshFactory::CreateCube(std::string name)
{
	std::vector<glm::vec3> verts
	{
		{-1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{-1.0f, -1.0f,  1.0f },
		{ 1.0f, -1.0f,  1.0f },

		{-1.0f,  1.0f, -1.0f },
		{ 1.0f,  1.0f, -1.0f },
		{-1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
	};

	std::vector<glm::vec4> colors(8, { 1, 1, 1, 1 });

	std::vector<uint16_t> indices
	{
		0, 1, 2, 3, 
		7, 1, 5, 0, 
		4, 2, 6, 7,
		4, 5,
	};

	auto newMesh = Factory::CreateInstance<Mesh>(name, ObjectType::MESH);
	newMesh->SetVertices(verts);
	newMesh->SetColors(colors);
	newMesh->SetIndices(indices);

	return newMesh;
}

Mesh* MeshFactory::CreateSphere(std::string name, int resolution)
{
	auto newMesh = Factory::CreateInstance<Mesh>(name, ObjectType::MESH);

	std::vector<glm::vec3> vertices;
	std::vector<uint16_t> indices;

	GeneratePlaneVertices(glm::vec3( 0.0f, -0.5f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::BOTTOM);
	newMesh->SetIndices(0, indices);

	int prevSize = indices.size();
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.5f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::TOP);
	newMesh->SetIndices(1, std::vector<glm::uint16_t>(indices.begin() + prevSize, indices.end()));

	prevSize = indices.size();
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.0f, -0.5f), resolution, resolution, vertices, indices, PlaneOrientation::FRONT);
	newMesh->SetIndices(2, std::vector<glm::uint16_t>(indices.begin() + prevSize, indices.end()));

	prevSize = indices.size();
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.0f,  0.5f), resolution, resolution, vertices, indices, PlaneOrientation::BACK);
	newMesh->SetIndices(3, std::vector<glm::uint16_t>(indices.begin() + prevSize, indices.end()));

	prevSize = indices.size();
	GeneratePlaneVertices(glm::vec3(-0.5f, 0.0f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::LEFT);
	newMesh->SetIndices(4, std::vector<glm::uint16_t>(indices.begin() + prevSize, indices.end()));

	prevSize = indices.size();
	GeneratePlaneVertices(glm::vec3( 0.5f, 0.0f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::RIGHT);
	newMesh->SetIndices(5, std::vector<glm::uint16_t>(indices.begin() + prevSize, indices.end()));

	prevSize = indices.size();
	GeneratePlaneVertices(glm::vec3( 0.0f, 0.5f,  0.0f), resolution, resolution, vertices, indices, PlaneOrientation::TOP);
	newMesh->SetIndices(6, std::vector<glm::uint16_t>(indices.begin() + prevSize, indices.end()));

	for (auto& vertex : vertices)
	{
		vertex = glm::normalize(vertex);
	}

	std::vector<glm::vec4> colors(vertices.size(), { 1, 1, 1, 1 });

	newMesh->SetVertices(vertices);
	newMesh->SetColors(colors);

	return newMesh;
}

Mesh * MeshFactory::CreateRectangle(std::string name)
{
	std::vector<glm::vec3> verts
	{
		{ -0.5f, -0.5f, 0 }, // bottom left - 0
		{  0.5f, -0.5f, 0 }, // bottom right - 1
		{ -0.5f,  0.5f, 0 }, // top left - 2
		{  0.5f,  0.5f, 0 }, // top right - 3

	};

	std::vector<glm::vec4> colors(6, { 1, 1, 1, 1 });

	std::vector<uint16_t> indices
	{
		2, 0, 3, 1
	};

	auto newMesh = Factory::CreateInstance<Mesh>(name, ObjectType::MESH);
	newMesh->SetVertices(verts);
	newMesh->SetColors(colors);
	newMesh->SetIndices(indices);

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
	float yy = 0.5f;

	float stepSizeX = 1.0f / (width - 1);
	float stepSizeY = -1.0f / (height - 1);

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

	int startIndex = indices.size() > 0 ? indices.back() + 1 : 0;

	for (int y = 0; y < height - 1; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			indices.push_back(startIndex + y * width + x);

			// Repeat first vertex of second row
			if (x == 0 && y != 0)
			{
				indices.push_back(indices.back());
			}

			indices.push_back(startIndex + y * width + x + width);

			// Repeat last vertex of first row (skip last row
			if (x == width - 1 && y != height - 2)
			{
				indices.push_back(indices.back());
			}
		}
	}
}
