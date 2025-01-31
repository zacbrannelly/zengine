#pragma once

#include "Collider3D.h"
#include "../Utilities/JsonHelpers.h"

namespace physx {
  class PxTriangleMesh;
}

namespace ZEngine
{
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

	#ifndef SWIG
		// Allow serialization / deserialization
		JSON_SCHEMA_BEGIN(MeshCollider3D)
			JSON_MAP_TO_SETTER_OPTIONAL(isTrigger, SetIsTrigger, bool)
		JSON_SCHEMA_END()
	#endif

	private:
		physx::PxTriangleMesh* _triangleMesh;
	};
}
