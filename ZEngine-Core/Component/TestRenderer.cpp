#include "TestRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

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
		{ -1.0f,  1.0f, 0 }, // top left
		{  1.0f,  1.0f, 0 }, // top right
		{ -1.0f, -1.0f, 0 }, // bottom left
		{  1.0f, -1.0f, 0 } // bottom right
	};

	vector<glm::vec4> colors
	{
		{ 0, 1, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 1, 0, 1 }
	};

	// CCW triangles
	std::vector<uint16_t> indices
	{
		0, 2, 1,
		2, 3, 1
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

void TestRenderer::Update()
{

}

void TestRenderer::Render(int viewId)
{
	_mesh->Draw(viewId, { _material }, glm::mat4(1.0f));
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
