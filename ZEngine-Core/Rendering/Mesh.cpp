#include "Mesh.h"
#include "DynamicIndexBuffer.h"
#include "DynamicVertexBuffer.h"
#include "Material.h"
#include "Graphics.h"
#include "Shader.h"

using namespace ZEngine;

Mesh::Mesh() : ZObject("Mesh", ObjectType::MESH)
{
	RegisterDerivedType(MESH);
	_vertexBuffer = nullptr;
	_colorBuffer = nullptr;
	_texCoordBuffer = nullptr;
	_normalBuffer = nullptr;
	_isDynamic = false;

	_vertexDecl.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.end();

	_colorDecl.begin()
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
		.end();

	_textureDecl.begin()
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();

	_normalDecl.begin()
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
		.end();
}

ZObject* Mesh::CreateInstance(std::string name, ObjectType type)
{
	return new Mesh();
}

void Mesh::SetDynamic(bool isDynamic)
{
	_isDynamic = isDynamic;
}

bool Mesh::IsDynamic() const
{
	return _isDynamic;
}

void Mesh::SetSubMeshes(const std::vector<SubMesh>& subMeshes)
{
	for (const auto& subMesh : subMeshes)
	{
		_subMeshes.push_back(new SubMesh(subMesh));
	}
}

void Mesh::SetIndices(const std::vector<uint32_t>& indices)
{
	SetIndices(0, indices);
}

void Mesh::SetIndices(int subMesh, const std::vector<uint32_t>& indices)
{
	assert(subMesh >= 0);

	if (subMesh >= _subMeshes.size())
	{
		auto newSubMesh = new SubMesh();
		newSubMesh->SetDynamic(_isDynamic);
		_subMeshes.push_back(newSubMesh);
		subMesh = _subMeshes.size() - 1;
	}

	_subMeshes[subMesh]->SetIndices(indices);
}

const std::vector<uint32_t>& Mesh::GetIndices()
{
	assert(_subMeshes.size() > 0);

	auto firstSubMesh = _subMeshes.front();
	return firstSubMesh->GetIndices();
}

const std::vector<uint32_t>& Mesh::GetIndices(int subMesh)
{
	assert(subMesh >= 0 && subMesh < _subMeshes.size());

	return _subMeshes[subMesh]->GetIndices();
}

int Mesh::GetSubMeshCount() const
{
	return _subMeshes.size();
}

void Mesh::SetMode(DrawMode mode)
{
	assert(_subMeshes.size() > 0);
	_subMeshes.front()->SetMode(mode);
}

void Mesh::SetMode(int subMesh, DrawMode mode)
{
	assert(subMesh >= 0 && subMesh < _subMeshes.size());
	_subMeshes[subMesh]->SetMode(mode);
}

DrawMode Mesh::GetMode() const
{
	assert(_subMeshes.size() > 0);
	return _subMeshes.front()->GetMode();
}

DrawMode Mesh::GetMode(int subMesh) const
{
	assert(_subMeshes.size() > subMesh);
	return _subMeshes[subMesh]->GetMode();
}

void Mesh::SetVertices(const float* data, unsigned int numVertices)
{
	SetBuffer<glm::vec3>(_vertexBuffer, _vertexDecl, _vertices, data, numVertices);
}

void Mesh::SetVertices(const std::vector<glm::vec3>& vertices)
{
	_vertices = vertices;
	SetBuffer<glm::vec3>(_vertexBuffer, _vertexDecl, _vertices);
}

const std::vector<glm::vec3>& Mesh::GetVertices() const
{
	return _vertices;
}

void Mesh::SetColors(const float* data, unsigned int numColors)
{
	SetBuffer<glm::vec4>(_colorBuffer, _colorDecl, _colors, data, numColors);
}

void Mesh::SetColors(const std::vector<glm::vec4>& colors)
{
	_colors = colors;
	SetBuffer<glm::vec4>(_colorBuffer, _colorDecl, _colors);
}

const std::vector<glm::vec4>& Mesh::GetColors() const
{
	return _colors;
}

void Mesh::SetNormals(const float* data, unsigned int numNormals)
{
	SetBuffer<glm::vec3>(_normalBuffer, _normalDecl, _normals, data, numNormals);
}

void Mesh::SetNormals(const std::vector<glm::vec3>& normals)
{
	_normals = normals;
	SetBuffer<glm::vec3>(_normalBuffer, _normalDecl, _normals);
}

const std::vector<glm::vec3>& Mesh::GetNormals() const
{
	return _normals;
}

void Mesh::SetTextureCoords(const float* data, unsigned int numCoords)
{
	SetBuffer<glm::vec2>(_texCoordBuffer, _textureDecl, _textureCoords, data, numCoords);
}

void Mesh::SetTextureCoords(const std::vector<glm::vec2>& texCoords)
{
	_textureCoords = texCoords;
	SetBuffer<glm::vec2>(_texCoordBuffer, _textureDecl, _textureCoords);
}

const std::vector<glm::vec2>& Mesh::GetTextureCoords() const
{
	return _textureCoords;
}

void Mesh::Draw(int viewId, const std::vector<Material*>& materials, glm::mat4& transform)
{
	auto graphics = Graphics::GetInstance();

	for (int i = 0; i < _subMeshes.size(); ++i)
	{
		auto material = i < materials.size() ? materials[i] : materials.size() > 0 ? materials.back() : nullptr;

		if (material != nullptr)
		{
			const auto& passes = material->GetShader()->GetPasses();

			for (const auto& pass : passes)
			{
				graphics->SetTransform(transform);
				graphics->SetVertexBuffer(0, _vertexBuffer);
				graphics->SetVertexBuffer(1, _colorBuffer);
				graphics->SetVertexBuffer(2, _texCoordBuffer);
				graphics->SetVertexBuffer(3, _normalBuffer);

				_subMeshes[i]->Draw(viewId, material, graphics, pass);
			}
		}
	}
}

Mesh::~Mesh()
{
	for (auto subMesh : _subMeshes)
	{
		delete subMesh;
	}

	delete _vertexBuffer;
	delete _colorBuffer;
	delete _texCoordBuffer;
	delete _normalBuffer;
}

SubMesh::SubMesh()
{
	_isDynamic = false;
	_drawMode = DrawMode::TRIANGLES;
}

SubMesh::SubMesh(const SubMesh& other)
{
	_indices = other._indices;
	_drawMode = other._drawMode;
	_isDynamic = other._isDynamic;
	_indexBuffer = other._indexBuffer;
}

void SubMesh::SetDynamic(bool isDynamic)
{
	_isDynamic = isDynamic;
}

bool SubMesh::IsDynamic() const
{
	return _isDynamic;
}

void SubMesh::SetMode(DrawMode mode)
{
	_drawMode = mode;
}

DrawMode SubMesh::GetMode() const
{
	return _drawMode;
}

void SubMesh::SetIndices(const std::vector<uint32_t>& indices)
{
	_indices = indices;

	if (_indexBuffer == nullptr)
		_indexBuffer = _isDynamic ? new DynamicIndexBuffer() : new IndexBuffer();

	if (_isDynamic && _indexBuffer->GetHandle().idx != bgfx::kInvalidHandle)
	{
		auto dynamicBuffer = static_cast<DynamicIndexBuffer*>(_indexBuffer);
		dynamicBuffer->Update(0, &indices[0], _indices.size() * sizeof(uint32_t), false);
	}
	else
	{
		_indexBuffer->Upload(&_indices[0], _indices.size() * sizeof(uint32_t), false);
	}
}

const std::vector<uint32_t>& SubMesh::GetIndices() const
{
	return _indices;
}

void SubMesh::Draw(int viewId, Material* material, Graphics* graphics, const Pass& pass)
{
	uint64_t renderFlags = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

	switch (_drawMode)
	{
	case LINES:
		renderFlags |= BGFX_STATE_PT_LINES;
		break;
	case LINE_STRIP:
		renderFlags |= BGFX_STATE_PT_LINESTRIP;
		break;
	case DRAW_MODE_POINTS:
		renderFlags |= BGFX_STATE_PT_POINTS;
		break;
	case TRIANGLE_STRIP:
		renderFlags |= BGFX_STATE_PT_TRISTRIP;
	}

	material->Apply();
	graphics->SetIndexBuffer(_indexBuffer);
	graphics->SetState(renderFlags | pass.state);

	graphics->Submit(viewId, pass.program);
}

SubMesh::~SubMesh()
{
	delete _indexBuffer;
}
