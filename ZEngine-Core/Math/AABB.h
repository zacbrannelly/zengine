#pragma once

#include <glm/glm.hpp>
#include "../Rendering/Mesh.h"

namespace ZEngine
{
  class AABB 
  {
  public:
    void* userData; // User data

    // Default constructor
    AABB() : _min(glm::vec3(FLT_MAX)), _max(glm::vec3(-FLT_MAX)), _meshMin(glm::vec3(FLT_MAX)), _meshMax(glm::vec3(-FLT_MAX)) {}

    // Constructor using min and max
    AABB(const glm::vec3 &min, const glm::vec3 &max) : _min(min), _max(max), _meshMin(min), _meshMax(max) {}

    void SetFromMesh(const Mesh* mesh, const glm::mat4& transform)
    {
      SetFromMesh(mesh->GetVertices(), transform);
    }

    void SetFromMesh(const std::vector<glm::vec3>& vertices, const glm::mat4& transform)
    {
      _meshMin = glm::vec3(FLT_MAX);
      _meshMax = glm::vec3(-FLT_MAX);

      for (const auto &vertex : vertices)
      {
        // Update min and max points
        _meshMin = glm::min(_meshMin, vertex);
        _meshMax = glm::max(_meshMax, vertex);
      }

      _min = _meshMin;
      _max = _meshMax;

      SetTransform(transform);
    }

    void SetTransform(const glm::mat4& transform)
    {
      _transform = transform;
      glm::vec3 corners[] = {
        glm::vec3(_meshMin.x, _meshMin.y, _meshMin.z),
        glm::vec3(_meshMin.x, _meshMin.y, _meshMax.z),
        glm::vec3(_meshMin.x, _meshMax.y, _meshMin.z),
        glm::vec3(_meshMin.x, _meshMax.y, _meshMax.z),
        glm::vec3(_meshMax.x, _meshMin.y, _meshMin.z),
        glm::vec3(_meshMax.x, _meshMin.y, _meshMax.z),
        glm::vec3(_meshMax.x, _meshMax.y, _meshMin.z),
        glm::vec3(_meshMax.x, _meshMax.y, _meshMax.z)
      };

      _min = glm::vec3(FLT_MAX);
      _max = glm::vec3(-FLT_MAX);

      for (const auto &corner : corners)
      {
        auto transformedCorner = glm::vec3(_transform * glm::vec4(corner, 1.0f));

        // Update min and max points
        _min = glm::min(_min, transformedCorner);
        _max = glm::max(_max, transformedCorner);
      }
    }

    bool Contains(const glm::vec3 &point) const
    {
      return (
        point.x >= _min.x && point.x <= _max.x &&
        point.y >= _min.y && point.y <= _max.y &&
        point.z >= _min.z && point.z <= _max.z
      );
    }

    AABB Copy() const
    {
      return AABB(_min, _max);
    }

    glm::vec3 GetMin() const
    {
      return _min;
    }

    glm::vec3 GetMax() const
    {
      return _max;
    }
      
  private:
    glm::vec3 _min; // Minimum corner of the AABB
    glm::vec3 _max; // Maximum corner of the AABB
    glm::vec3 _meshMin; // Original minimum corner of the AABB
    glm::vec3 _meshMax; // Original maximum corner of the AABB
    glm::mat4 _transform; // Transform of the AABB
  };
}
