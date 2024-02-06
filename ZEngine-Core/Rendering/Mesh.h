#pragma once

#include "../Map/Objects/ZObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <bgfx/bgfx.h>
#include "../Utilities/JsonHelpers.h"

class Material;
class VertexBuffer;
class DynamicVertexBuffer;
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
	SubMesh();
	SubMesh(const SubMesh& other);
	~SubMesh();

	void SetMode(DrawMode mode);
	DrawMode GetMode() const;

	void SetIndices(const std::vector<uint32_t>& indices);
	const std::vector<uint32_t>& GetIndices() const;

	void SetDynamic(bool isDynamic);
	bool IsDynamic() const;

	void Draw(int viewId, Material* material, Graphics* graphics, const Pass& pass);

	JSON_SCHEMA_BEGIN(SubMesh)
		JSON_TO_SETTER          ("indices", SetIndices, std::vector<uint32_t>)
		JSON_TO_SETTER          ("mode",    SetMode,    DrawMode)
		JSON_TO_SETTER_OPTIONAL ("dynamic", SetDynamic, bool)
	JSON_SCHEMA_END()

private:
	std::vector<uint32_t> _indices;
	IndexBuffer* _indexBuffer;
	DrawMode _drawMode;
	bool _isDynamic;
};

class Mesh : public ZObject
{
public:
	Mesh();
	~Mesh();

	void SetDynamic(bool isDynamic);
	bool IsDynamic() const;

	void SetIndices(const std::vector<uint32_t>& indices);
	void SetIndices(int subMesh, const std::vector<uint32_t>& indices);
	const std::vector<uint32_t>& GetIndices();
	const std::vector<uint32_t>& GetIndices(int subMesh);

	void SetSubMeshes(const std::vector<SubMesh>& subMeshes);
	int GetSubMeshCount() const;

	void SetMode(DrawMode mode);
	void SetMode(int subMesh, DrawMode mode);
	DrawMode GetMode() const;
	DrawMode GetMode(int subMesh) const;

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
	void SetBuffer(VertexBuffer*& buffer, bgfx::VertexLayout& decl, std::vector<T>& originalData, const float* data, unsigned int numElements);

	template<typename T>
	void SetBuffer(VertexBuffer*& buffer, bgfx::VertexLayout& decl, const std::vector<T>& data);

	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec4> _colors;
	std::vector<glm::vec2> _textureCoords;
	std::vector<glm::vec3> _normals;
	
	std::vector<SubMesh*> _subMeshes;

	VertexBuffer* _vertexBuffer;
	VertexBuffer* _colorBuffer;
	VertexBuffer* _normalBuffer;
	VertexBuffer* _texCoordBuffer;

	// TODO: Stop using bgfx::VertexLayout and use our own VertexLayout class
	bgfx::VertexLayout _vertexDecl;
	bgfx::VertexLayout _colorDecl;
	bgfx::VertexLayout _textureDecl;
	bgfx::VertexLayout _normalDecl;

	bool _isDynamic;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
	
	static ObjectType GetStaticType()
	{
		return MESH;
	}

	JSON_SCHEMA_BEGIN(Mesh)
		JSON_TO_SETTER          ("vertices",  SetVertices,      std::vector<glm::vec3>)
		JSON_TO_SETTER          ("indices",   SetIndices,       std::vector<uint32_t>)
		JSON_TO_SETTER_OPTIONAL ("colors",    SetColors,        std::vector<glm::vec4>)
		JSON_TO_SETTER_OPTIONAL ("texCoords", SetTextureCoords, std::vector<glm::vec2>)
		JSON_TO_SETTER_OPTIONAL ("normals",   SetNormals,       std::vector<glm::vec3>)
		JSON_TO_SETTER_OPTIONAL ("subMeshes", SetSubMeshes,     std::vector<SubMesh>)
	JSON_SCHEMA_END()
};

#include "./VertexBuffer.h"
#include "./DynamicVertexBuffer.h"

template<typename T>
inline void Mesh::SetBuffer(VertexBuffer*& buffer, bgfx::VertexLayout& decl, std::vector<T>& originalData, const float* data, unsigned int numElements)
{
	originalData.resize(numElements, T());
	memcpy(&originalData[0], data, sizeof(T) * numElements);

	SetBuffer<T>(buffer, decl, originalData);
}

template<typename T>
inline void Mesh::SetBuffer(VertexBuffer*& buffer, bgfx::VertexLayout& decl, const std::vector<T>& data)
{
	if (data.size() == 0) return;

	if (buffer == nullptr)
		buffer = _isDynamic	
			? new DynamicVertexBuffer(decl) 
			: new VertexBuffer(decl);

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
