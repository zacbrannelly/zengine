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

TestRenderer::TestRenderer() : Component("Test Renderer", ObjectType::TEST_RENDERER)
{
}

void TestRenderer::Init()
{
	bgfx::VertexDecl decl;
	decl.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
	.end();

	struct vert
	{
		glm::vec3 pos;
		float r, g, b, a;
	};

	vert vertices[] =
	{
		{ { -1.0f,  1.0f, 0 }, 0, 1, 0, 1 }, // top left
		{ {  1.0f,  1.0f, 0 }, 0, 1, 0, 1 }, // top right
		{ { -1.0f, -1.0f, 0 }, 0, 1, 0, 1 }, // bottom left
		{ {  1.0f, -1.0f, 0 }, 0, 1, 0, 1 } // bottom right
	};

	// CCW triangles!! (since we cull CW)
	uint16_t indices[] =
	{
		0, 2, 1,
		2, 3, 1
	};

	_vertexBuffer = new VertexBuffer(decl);
	_indexBuffer = new IndexBuffer();

	_vertexBuffer->Upload(vertices, sizeof(vertices), true);
	_indexBuffer->Upload(indices, sizeof(indices), true);

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
	auto g = Graphics::GetInstance();
	auto transform = glm::mat4(1.0f);

	g->SetTransform(transform);

	g->SetVertexBuffer(0, _vertexBuffer);
	g->SetIndexBuffer(_indexBuffer);

	g->SetState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);
	g->Submit(viewId, _material);
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
