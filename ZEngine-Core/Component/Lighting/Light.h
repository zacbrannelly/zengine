#pragma once

#include "../Component.h"

namespace ZEngine
{
  class Light : public Component
  {
  public:
    Light(std::string name, ObjectType objectType) : Component(name, objectType)
    {
      RegisterDerivedType(LIGHT);
    }
    virtual ~Light() {}

    static ObjectType GetStaticType()
    {
      return LIGHT;
    }
  };
}
