#include "Ray.h"
#include "../Component/Camera.h"
#include "../Map/Objects/Entity.h"
#include "../Component/Transform.h"

using namespace ZEngine;

Ray Ray::FromScreenPos(int mouseX, int mouseY, const Camera* camera)
{
  // Convert screen coordinates to Normalized Device Coordinates (NDC).
  glm::vec2 ndc = Ray::GetNDC(mouseX, mouseY, camera);

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

glm::vec2 Ray::GetNDC(int mouseX, int mouseY, const Camera* camera)
{
  float x = (2.0f * mouseX) / camera->GetViewportWidth() - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / camera->GetViewportHeight();
  return glm::vec2(x, y);
}
