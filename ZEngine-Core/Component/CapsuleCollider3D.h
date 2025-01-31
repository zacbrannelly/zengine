#pragma once

#include "DynamicCollider3D.h"
#include "../Utilities/JsonHelpers.h"
#include <glm/glm.hpp>

namespace ZEngine
{
	class CapsuleCollider3D : public DynamicCollider3D
	{
	public:
		CapsuleCollider3D();
		~CapsuleCollider3D();

		void BuildGeometry() override;

		void SetRadius(float radius);
		float GetRadius() const;

		void SetHeight(float height);
		float GetHeight() const;

		void SetSize(float radius, float height);

		float GetWorldRadius() const;
		float GetWorldHeight() const;

		static ZObject* CreateInstance(std::string name, ObjectType type);
		static ZObject* Copy(std::string name, ZObject* object);

		static ObjectType GetStaticType()
		{
			return CAPSULE_COLLIDER_3D;
		}

	#ifndef SWIG
		// Allow serialization / deserialization
		JSON_SCHEMA_BEGIN(CapsuleCollider3D)
			JSON_MAP_TO_SETTER_OPTIONAL (isTrigger, SetIsTrigger, bool)
			JSON_MAP_TO_SETTER_OPTIONAL (radius,    SetRadius,    float)
			JSON_MAP_TO_SETTER_OPTIONAL (height,    SetHeight,    float)
		JSON_SCHEMA_END()
	#endif

	private:
		float _radius;
		float _height;
	};
}
