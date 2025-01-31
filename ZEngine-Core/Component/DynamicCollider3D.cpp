#include "DynamicCollider3D.h"

using namespace ZEngine;

DynamicCollider3D::DynamicCollider3D(std::string name, ObjectType objectType) : Collider3D(name, objectType)
{
  RegisterDerivedType(DYNAMIC_COLLIDER_3D);
}

DynamicCollider3D::~DynamicCollider3D()
{
}
