#include "MeshRenderer.h"
#include "Transform.h"
#include "../Map/Objects/Entity.h"
#include "../Rendering/Mesh.h"

using namespace std;

MeshRenderer::MeshRenderer() : Component("Mesh Renderer", ObjectType::MESH_RENDERER)
{
	_mesh = nullptr;
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	_mesh = mesh;
}

Mesh* MeshRenderer::GetMesh() const
{
	return _mesh;
}

void MeshRenderer::SetMaterial(Material* material)
{
	if (_materials.size() == 0)
	{
		_materials.push_back(material);
	}
	else
	{
		_materials[0] = material;
	}
}

Material* MeshRenderer::GetMaterial() const
{
	return _materials.size() > 0 ? _materials.front() : nullptr;
}

void MeshRenderer::SetMaterials(const vector<Material*>& materials)
{
	_materials = materials;
}

const vector<Material*>& MeshRenderer::GetMaterials() const
{
	return _materials;
}

void MeshRenderer::Init()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::Render(int viewId)
{
	if (_mesh != nullptr)
	{
		auto transform = GetOwner()->GetTransform()->GetWorldTransformMatrix();
		_mesh->Draw(viewId, _materials, transform);
	}
}

MeshRenderer::~MeshRenderer()
{
}

ZObject* MeshRenderer::CreateInstance(std::string name, ObjectType type)
{
	return new MeshRenderer();
}