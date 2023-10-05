#pragma once

#include "Collider3D.h"
#include <glm/glm.hpp>

class PlaneCollider3D : public Collider3D
{
public:
	PlaneCollider3D();
	~PlaneCollider3D();

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return PLANE_COLLIDER_3D;
	}

private:
	glm::vec3 _direction;
  float _distance;
};
