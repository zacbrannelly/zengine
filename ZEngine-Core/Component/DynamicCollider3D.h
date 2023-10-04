#pragma once

#include "Collider3D.h"

class DynamicCollider3D : public Collider3D
{
public:
  DynamicCollider3D(std::string name, ObjectType objectType);
  virtual ~DynamicCollider3D();

  static ObjectType GetStaticType() 
  {
    return DYNAMIC_COLLIDER_3D;
  }
};
