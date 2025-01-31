#pragma once

#include "Component.h"

#include <bgfx/bgfx.h>

namespace ZEngine
{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Material;
	class Mesh;

	class TestRenderer : public Component 
	{
	public:
		TestRenderer();
		~TestRenderer();

		void Init() override;
		void Update() override;
		void Render(int viewId) override;

		static ZObject* CreateInstance(std::string name, ObjectType type);

		static ObjectType GetStaticType()
		{
			return TEST_RENDERER;	
		}

	private:
		VertexBuffer* _vertexBuffer;
		IndexBuffer* _indexBuffer;
		Shader* _program;
		Material* _material;
		Mesh* _mesh;
	};
}
