#pragma once

#define NDEBUG
#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

inline physx::PxVec3 ToPxVec3(const glm::vec3& vec)
{
  return physx::PxVec3(vec.x, vec.y, vec.z);
}

inline glm::vec3 ToGlmVec3(const physx::PxVec3& vec)
{
  return glm::vec3(vec.x, vec.y, vec.z);
}
