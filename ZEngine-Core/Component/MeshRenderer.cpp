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

void MeshRenderer::SetMaterial(Material* material, int index)
{
	if (index < 0 || index >= _materials.size())
	{
		_materials.push_back(material);
	}
	else
	{
		_materials[index] = material;
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

ZObject* MeshRenderer::Copy(string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != MESH_RENDERER)
		return nullptr;

	auto source = static_cast<MeshRenderer*>(object);
	auto copy = new MeshRenderer();

	copy->SetMaterials(source->GetMaterials());
	copy->SetMesh(source->GetMesh());

	return copy;
}
