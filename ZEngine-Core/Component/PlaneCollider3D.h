#pragma once

#include "Collider3D.h"
#include "../Utilities/JsonHelpers.h"
#include <glm/glm.hpp>

class PlaneCollider3D : public Collider3D
{
public:
	PlaneCollider3D();
	~PlaneCollider3D();

	virtual void BuildGeometry() override;

	void SetDirection(glm::vec3 direction);
	const glm::vec3& GetDirection() const;

	void SetDistance(float distance);
	float GetDistance() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return PLANE_COLLIDER_3D;
	}

#ifndef SWIG
	// Allow serialization / deserialization
	JSON_SCHEMA_BEGIN(PlaneCollider3D)
		JSON_MAP_TO_SETTER_OPTIONAL (isTrigger, SetIsTrigger, bool)
		JSON_MAP_TO_SETTER_OPTIONAL (direction, SetDirection, glm::vec3)
		JSON_MAP_TO_SETTER_OPTIONAL (distance,  SetDistance,  float)
	JSON_SCHEMA_END()
#endif

private:
	glm::vec3 _direction;
  float _distance;
};
