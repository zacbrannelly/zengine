#include "TestRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

#include "../Map/Objects/Entity.h"
#include "../Component/Transform.h"

#include "../Rendering/Graphics.h"
#include "../Rendering/Shader.h"
#include "../Rendering/VertexBuffer.h"
#include "../Rendering/IndexBuffer.h"
#include "../Rendering/Material.h"
#include "../Rendering/Mesh.h"

using namespace std;

TestRenderer::TestRenderer() : Component("Test Renderer", ObjectType::TEST_RENDERER)
{
}

void TestRenderer::Init()
{
	vector<glm::vec3> verts
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

	vector<glm::vec4> colors
	{
		{ 0, 1, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 1, 0, 1 }, 

		{ 1, 0, 0, 1 },
		{ 1, 0, 0, 1 },
		{ 1, 0, 0, 1 },
		{ 1, 0, 0, 1 },
	};

	// CCW triangles
	std::vector<uint16_t> indices
	{
			0, 1, 2,
			3,
			7,
			1,
			5,
			0,
			4,
			2,
			6,
			7,
			4,
			5,
	};

	_mesh = new Mesh("test mesh");
	_mesh->SetDynamic(false);

	_mesh->SetVertices(verts);
	_mesh->SetColors(colors);
	_mesh->SetIndices(indices);

	_program = new Shader("cubes");
	_program->Load("vs_cubes.bin", "fs_cubes.bin");

	_material = new Material("test material");
	_material->SetShader(_program);
}

float g_counter = 0.0f;

void TestRenderer::Update()
{
	
}

void TestRenderer::Render(int viewId)
{
	auto transform = GetOwner()->GetTransform();

	transform->SetRotation({ 0, g_counter, 0 });

	g_counter += 0.01f;

	_mesh->Draw(viewId, { _material }, GetOwner()->GetTransform()->GetWorldTransformMatrix());
}

ZObject* TestRenderer::CreateInstance(std::string name, ObjectType type)
{
	return new TestRenderer();
}

TestRenderer::~TestRenderer()
{
	delete _vertexBuffer;
	delete _indexBuffer;
	delete _material;
	delete _program;
}
