#pragma once

#include "Collider3D.h"

namespace physx {
  class PxTriangleMesh;
}

class MeshCollider3D : public Collider3D
{
public:
	MeshCollider3D();
	~MeshCollider3D();

	virtual void BuildGeometry() override;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return MESH_COLLIDER_3D;
	}

private:
	physx::PxTriangleMesh* _triangleMesh;
};
