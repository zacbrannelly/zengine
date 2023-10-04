#pragma once

#include "DynamicCollider3D.h"
#include <glm/glm.hpp>

class CapsuleCollider3D : public DynamicCollider3D
{
public:
	CapsuleCollider3D();
	~CapsuleCollider3D();

  void SetSize(float radius, float height);
  float GetRadius() const;
  float GetHeight() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return BOX_COLLIDER_3D;
	}

private:
	float _radius;
  float _height;
};
