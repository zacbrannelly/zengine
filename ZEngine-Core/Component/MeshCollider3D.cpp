#include "MeshCollider3D.h"
#include "../Physics/Physics3DSystem.h"
#include "../Rendering/Mesh.h"
#include "../Map/Objects/Entity.h"
#include "MeshRenderer.h"
#include "Transform.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

using namespace physx;
using namespace ZEngine;

MeshCollider3D::MeshCollider3D() : Collider3D("Mesh Collider 3D", MESH_COLLIDER_3D)
{
  RegisterDerivedType(MESH_COLLIDER_3D);
}

void MeshCollider3D::BuildGeometry()
{
  const auto owner = GetOwner();
  if (owner == nullptr)
  {
    throw std::runtime_error("MeshCollider3D must be attached to an Entity before Init!");
  }

  // TODO: Allow selecting which mesh to use instead of finding one on the owner.
  const auto meshRenderer = owner->GetComponentByType<MeshRenderer>();
  if (meshRenderer == nullptr)
  {
    throw std::runtime_error("MeshCollider3D must be attached to an Entity with a MeshRenderer before Init!");
  }

  const auto mesh = meshRenderer->GetMesh();
  const auto verticesGlm = mesh->GetVertices();
  const auto indices = mesh->GetIndices();

  // Convert vertices to PhysX format.
  const auto numVertices = verticesGlm.size();
  const auto vertices = new PxVec3[numVertices];
  
  // TODO: See if we can just memcpy the vertices.
  for (auto i = 0; i < numVertices; ++i)
  {
    vertices[i] = PxVec3(verticesGlm[i].x, verticesGlm[i].y, verticesGlm[i].z);
  }

  PxTolerancesScale scale;
  PxCookingParams params(scale);
  // disable mesh cleaning - perform mesh validation on development configurations
  params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
  // disable edge precompute, edges are set for each triangle, slows contact generation
  params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

  PxTriangleMeshDesc meshDesc;
  meshDesc.points.count = numVertices;
  meshDesc.points.stride = sizeof(PxVec3);
  meshDesc.points.data = vertices;

  meshDesc.triangles.count = indices.size() / 3;
  meshDesc.triangles.stride = 3 * sizeof(uint32_t);
  meshDesc.triangles.data = indices.data();
  
  // Flips normals - seems the winding order is different between PhysX and BGFX.
  meshDesc.flags = PxMeshFlag::eFLIPNORMALS;

  auto thePhysics = Physics3DSystem::GetInstance()->GetPhysics();
  auto theCooking = Physics3DSystem::GetInstance()->GetCooking();

  _triangleMesh = PxCreateTriangleMesh(
    params,
    meshDesc,
    thePhysics->getPhysicsInsertionCallback()
  );

  if (!_triangleMesh)
  {
    throw std::runtime_error("Failed to create triangle mesh!");
  }

  const auto& transformScale = owner->GetTransform()->GetScale();
  const PxVec3 meshScale(transformScale.x, transformScale.y, transformScale.z);
  SetGeometrySilently(new PxTriangleMeshGeometry(_triangleMesh, PxMeshScale(meshScale)));
}

ZObject* MeshCollider3D::CreateInstance(std::string name, ObjectType type)
{
  return new MeshCollider3D();
}

ZObject* MeshCollider3D::Copy(std::string name, ZObject* object)
{
  auto source = static_cast<MeshCollider3D*>(object);
  auto copy = new MeshCollider3D();
  copy->SetIsTrigger(source->IsTrigger());

  return copy;
}

MeshCollider3D::~MeshCollider3D()
{
  PX_RELEASE(_triangleMesh);
}
