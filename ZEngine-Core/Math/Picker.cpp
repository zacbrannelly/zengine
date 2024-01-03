#include "Picker.h"
#include "Ray.h"
#include "AABB.h"
#include "../Component/Camera.h"
#include "../Map/Objects/Entity.h"
#include "../Component/Transform.h"

const AABB* Picker::PickAABB(const std::vector<AABB>& aabbs, const Ray& ray)
{
  const AABB* pickedBox = nullptr;
  float minDistance = FLT_MAX;

  for (const auto& box : aabbs)
  {
    float distance;
    if (RayIntersectsAABB(ray, box, distance) && distance < minDistance)
    {
      minDistance = distance;
      pickedBox = &box;
    }
  }

  return pickedBox;
}

bool Picker::RayIntersectsAABB(const Ray& ray, const AABB& box, float& intersectionDistance)
{
  float tMin = 0.0f; // set to -FLT_MAX for negative values
  float tMax = FLT_MAX;

  const glm::vec3 boxBounds[2] = {box.GetMin(), box.GetMax()};

  for (int i = 0; i < 3; ++i) { // For each axis
    if (std::abs(ray.direction[i]) < std::numeric_limits<float>::epsilon())
    {
      // Ray is parallel to slab. No hit if origin not within slab
      if (ray.origin[i] < boxBounds[0][i] || ray.origin[i] > boxBounds[1][i]) 
        return false;
    }
    else
    {
      // Compute intersection t value of ray with near and far plane of slab
      float ood = 1.0f / ray.direction[i];
      float t1 = (boxBounds[0][i] - ray.origin[i]) * ood;
      float t2 = (boxBounds[1][i] - ray.origin[i]) * ood;

      // Make t1 be intersection with near plane, t2 with far plane
      if (t1 > t2) std::swap(t1, t2);

      // Compute the intersection of slab intersection intervals
      tMin = std::max(tMin, t1);
      tMax = std::min(tMax, t2);

      // Exit with no collision as soon as slab intersection becomes empty
      if (tMin > tMax) return false;
    }
  }

  // Ray intersects all 3 slabs. Return point (tMin) of first intersection
  intersectionDistance = tMin;
  return true;
}

Ray Picker::GetRayFromScreenPos(int mouseX, int mouseY, const Camera* camera)
{
  // Convert screen coordinates to NDC
  glm::vec2 ndc = GetNDC(mouseX, mouseY, camera);

  // Unproject NDC to world space
  glm::vec4 rayStartNDC(ndc.x, ndc.y, -1.0, 1.0); // Near plane
  glm::vec4 rayEndNDC(ndc.x, ndc.y, 1.0, 1.0);   // Far plane

  glm::mat4 inverseProjectionView = glm::inverse(camera->GetProjectionMatrix() * camera->GetViewMatrix());
  glm::vec4 rayStartWorld = inverseProjectionView * rayStartNDC;
  glm::vec4 rayEndWorld = inverseProjectionView * rayEndNDC;

  rayStartWorld /= rayStartWorld.w;
  rayEndWorld /= rayEndWorld.w;

  glm::vec3 rayDir = glm::normalize(glm::vec3(rayEndWorld - rayStartWorld));

  if (camera->GetProjectionMode() == Camera::ProjectionMode::ORTHOGRAPHIC)
  {
    // For orthographic projection, use the camera's forward direction
    rayDir = camera->GetOwner()->GetTransform()->GetForward();
  }

  return Ray(glm::vec3(rayStartWorld), rayDir);
}

glm::vec2 Picker::GetNDC(int mouseX, int mouseY, const Camera* camera)
{
  float x = (2.0f * mouseX) / camera->GetViewportWidth() - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / camera->GetViewportHeight();
  return glm::vec2(x, y);
}
