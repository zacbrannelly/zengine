#pragma once

#include "Collider3D.h"
#include <glm/glm.hpp>

class BoxCollider3D : public Collider3D
{
public:
	BoxCollider3D();
	~BoxCollider3D();

  void SetSize(glm::vec3 size);
  glm::vec3 GetSize() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

  static ObjectType GetStaticType()
  {
    return BOX_COLLIDER_3D;
  }

private:
  glm::vec3 _size;
};
