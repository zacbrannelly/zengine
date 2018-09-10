#include "TestRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include "../Rendering/Graphics.h"

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

	uint16_t indices[] =
	{
		0, 2, 1,
		2, 3, 1
	};

	auto vertexBufferMem = bgfx::copy(&vertices, sizeof(vertices));
	auto indexBufferMem = bgfx::copy(&indices, sizeof(indices));

	_vertexBuffer = bgfx::createVertexBuffer(vertexBufferMem, decl);
	_indexBuffer = bgfx::createIndexBuffer(indexBufferMem);

	bgfx::ShaderHandle vertexShader;
	bgfx::ShaderHandle fragShader;

	std::ifstream in("vs_cubes.bin", std::ifstream::in | std::ifstream::binary | std::ifstream::ate);

	if (in.is_open())
	{
		std::ifstream::pos_type size = in.tellg();
		in.seekg(0, std::ios::beg);

		unsigned int allocSize = (unsigned int)size + 1;

		auto buffer = bgfx::alloc(allocSize);
		in.read((char*)buffer->data, size);

		// Null terminate
		buffer->data[allocSize - 1] = '\0';

		vertexShader = bgfx::createShader(buffer);
	}

	in.close();

	in = std::ifstream("fs_cubes.bin", std::ios::in | std::ios::binary | std::ios::ate);

	if (in.is_open())
	{
		std::ifstream::pos_type size = in.tellg();
		in.seekg(0, std::ios::beg);

		unsigned int allocSize = (unsigned int)size + 1;

		auto buffer = bgfx::alloc(allocSize);
		in.read((char*)buffer->data, size);

		// Null terminate
		buffer->data[allocSize - 1] = '\0';

		fragShader = bgfx::createShader(buffer);
	}

	in.close();

	_program = bgfx::createProgram(vertexShader, fragShader, true);
}

void TestRenderer::Update()
{

}

void TestRenderer::Render(int viewId)
{
	auto g = Graphics::GetInstance();
	auto transform = glm::mat4(1.0f);

	g->SetTransform(transform);

	// TODO: Abstract these into their classes
	bgfx::setVertexBuffer(viewId, _vertexBuffer);
	bgfx::setIndexBuffer(_indexBuffer);

	g->SetState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);
	g->Submit(viewId, _program);
}

ZObject* TestRenderer::CreateInstance(std::string name, ObjectType type)
{
	return new TestRenderer();
}

TestRenderer::~TestRenderer()
{
}
