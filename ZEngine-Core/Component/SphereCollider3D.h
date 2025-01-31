#pragma once

#include "DynamicCollider3D.h"
#include "../Utilities/JsonHelpers.h"
#include <glm/glm.hpp>

namespace ZEngine
{
	class SphereCollider3D : public DynamicCollider3D
	{
	public:
		SphereCollider3D();
		~SphereCollider3D();

		void BuildGeometry() override;

		void SetRadius(float radius);
		float GetRadius() const;
		float GetWorldRadius() const;

		static ZObject* CreateInstance(std::string name, ObjectType type);
		static ZObject* Copy(std::string name, ZObject* object);

		static ObjectType GetStaticType()
		{
			return SPHERE_COLLIDER_3D;
		}

	#ifndef SWIG
		// Allow serialization / deserialization
		JSON_SCHEMA_BEGIN(SphereCollider3D)
			JSON_MAP_TO_SETTER_OPTIONAL (isTrigger, SetIsTrigger, bool)
			JSON_MAP_TO_SETTER_OPTIONAL (radius,    SetRadius,    float)
		JSON_SCHEMA_END()
	#endif

	private:
		float _radius;
	};
}
