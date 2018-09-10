#pragma once

#include "Component.h"
#include <bgfx/bgfx.h>

class TestRenderer : public Component 
{
public:
	TestRenderer();
	~TestRenderer();

	void Init() override;
	void Update() override;
	void Render(int viewId) override;

	static ZObject* CreateInstance(std::string name, ObjectType type);

private:
	bgfx::VertexBufferHandle _vertexBuffer;
	bgfx::IndexBufferHandle _indexBuffer;
	bgfx::ProgramHandle _program;
};

