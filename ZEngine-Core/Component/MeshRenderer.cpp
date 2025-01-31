#include "MeshRenderer.h"
#include "Transform.h"
#include "../Map/Objects/Entity.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/MeshFactory.h"

using namespace std;
using namespace ZEngine;

MeshRenderer::MeshRenderer() : Component("Mesh Renderer", ObjectType::MESH_RENDERER)
{
	RegisterDerivedType(MESH_RENDERER);
	_mesh = nullptr;
}

void MeshRenderer::SetMeshFromAsset(ModelAsset* modelAsset)
{
	_mesh = modelAsset->GetMesh();
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

void MeshRenderer::SetMaterialsFromAssets(const vector<MaterialAsset*>& materialAssets)
{
	for (auto materialAsset : materialAssets)
	{
		_materials.push_back(materialAsset->GetMaterial());
	}
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
    Render(viewId, GetOwner()->GetTransform());
}

void MeshRenderer::Render(int viewId, Transform* transform)
{
	if (_mesh != nullptr)
	{
		auto worldTransform = transform->GetWorldTransformMatrix();
		_mesh->Draw(viewId, _materials, worldTransform);
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

void MeshRenderer::OnDeserialization(const nlohmann::json& in, MeshRenderer& out)
{
	// Build the mesh from a primitive
	// TODO: Put this into a struct and define a schema for it.
	if (in.contains("primitive"))
	{
		auto primitive = in["primitive"].get<string>();

		if (primitive == "cube")
		{
			out.SetMesh(MeshFactory::CreateCube("Cube"));
		}
		else if (primitive == "sphere")
		{
			out.SetMesh(MeshFactory::CreateSphereStrip("Sphere"));
		}
		else if (primitive == "rectangle")
		{
			out.SetMesh(MeshFactory::CreateRectangle("Rectangle"));
		}
		else if (primitive == "plane")
		{
			int width = 10;
			int height = 10;
			PlaneOrientation facing = FRONT;

			if (in.contains("plane_size"))
			{
				auto data = in["plane_size"].get<vector<int>>();
				width = data[0];
				height = data[1];
			}

			if (in.contains("plane_facing"))
			{
				facing = (PlaneOrientation)in["plane_facing"].get<unsigned int>();
			}

			out.SetMesh(MeshFactory::CreatePlane("Plane", width, height, facing));
		}
	}
}
