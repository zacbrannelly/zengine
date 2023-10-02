#pragma once

#include "Collider3D.h"
#include <glm/glm.hpp>

class SphereCollider3D : public Collider3D
{
public:
	SphereCollider3D();
	~SphereCollider3D();

	void SetRadius(float radius);
	float GetRadius() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return SPHERE_COLLIDER_3D;
	}

private:
	float _radius;
};
