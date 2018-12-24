#pragma once

#include "../Map/Objects/ZObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <bgfx/bgfx.h>

class Material;
class VertexBuffer;
class IndexBuffer;
class Graphics;
struct Pass;

enum DrawMode
{
	TRIANGLES, TRIANGLE_STRIP, LINES, LINE_STRIP, DRAW_MODE_POINTS
};

class SubMesh
{
public:
	SubMesh(bool isDynamic);
	~SubMesh();

	void SetMode(DrawMode mode);
	DrawMode GetMode() const;

	void SetIndices(const std::vector<uint32_t>& indices);
	const std::vector<uint32_t>& GetIndices() const;

	void Draw(int viewId, Material* material, Graphics* graphics, const Pass& pass);

private:
	std::vector<uint32_t> _indices;
	IndexBuffer* _indexBuffer;
	DrawMode _drawMode;
	bool _isDynamic;
};

class Mesh : public ZObject
{
public:
	Mesh(std::string name);
	~Mesh();

	void SetDynamic(bool isDynamic);
	bool IsDynamic() const;

	void SetIndices(const std::vector<uint32_t>& indices);
	void SetIndices(int subMesh, const std::vector<uint32_t>& indices);
	const std::vector<uint32_t>& GetIndices();
	const std::vector<uint32_t>& GetIndices(int subMesh);
	int GetSubMeshCount() const;

	void SetMode(DrawMode mode);
	void SetMode(int subMesh, DrawMode mode);

	void SetVertices(const float* data, unsigned int numVertices);
	void SetVertices(const std::vector<glm::vec3>& vertices);
	const std::vector<glm::vec3>& GetVertices() const;
	
	void SetColors(const float* data, unsigned int numColors);
	void SetColors(const std::vector<glm::vec4>& colors);
	const std::vector<glm::vec4>& GetColors() const;
	
	void SetTextureCoords(const float* data, unsigned int numCoords);
	void SetTextureCoords(const std::vector<glm::vec2>& coords);
	const std::vector<glm::vec2>& GetTextureCoords() const;

	void SetNormals(const float* data, unsigned int numNormals);
	void SetNormals(const std::vector<glm::vec3>& vertices);
	const std::vector<glm::vec3>& GetNormals() const;

	void Draw(int viewId, const std::vector<Material*>& materials, glm::mat4& transform);
private:
	template<typename T>
	void SetBuffer(VertexBuffer*& buffer, bgfx::VertexDecl& decl, std::vector<T>& originalData, const float* data, unsigned int numElements);

	template<typename T>
	void SetBuffer(VertexBuffer*& buffer, bgfx::VertexDecl& decl, const std::vector<T>& data);

	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec4> _colors;
	std::vector<glm::vec2> _textureCoords;
	std::vector<glm::vec3> _normals;
	
	std::vector<SubMesh*> _subMeshes;

	VertexBuffer* _vertexBuffer;
	VertexBuffer* _colorBuffer;
	VertexBuffer* _normalBuffer;
	VertexBuffer* _texCoordBuffer;

	bgfx::VertexDecl _vertexDecl;
	bgfx::VertexDecl _colorDecl;
	bgfx::VertexDecl _textureDecl;
	bgfx::VertexDecl _normalDecl;

	bool _isDynamic;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
};

template<typename T>
inline void Mesh::SetBuffer(VertexBuffer*& buffer, bgfx::VertexDecl& decl, std::vector<T>& originalData, const float* data, unsigned int numElements)
{
	originalData.resize(numElements, T());
	memcpy(&originalData[0], data, sizeof(T) * numElements);

	SetBuffer<T>(buffer, decl, originalData);
}

template<typename T>
inline void Mesh::SetBuffer(VertexBuffer*& buffer, bgfx::VertexDecl& decl, const std::vector<T>& data)
{
	if (data.size() == 0) return;

	if (buffer == nullptr)
	{
		if (_isDynamic)
			buffer = new DynamicVertexBuffer(decl);
		else
			buffer = new VertexBuffer(decl);
	}

	if (_isDynamic && buffer->GetHandle().idx != bgfx::kInvalidHandle)
	{
		auto dynamicVertexBuffer = static_cast<DynamicVertexBuffer*>(buffer);
		dynamicVertexBuffer->Update(0, &data[0], _vertices.size() * sizeof(T), false);
	}
	else
	{
		buffer->Upload(&data[0], data.size() * sizeof(T), false);
	}
}
