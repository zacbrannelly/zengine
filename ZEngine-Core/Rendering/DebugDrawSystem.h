#pragma once

#include "../Misc/Singleton.h"
#include "../Map/Objects/ObjectType.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace ZEngine
{
  class Material;
  class Mesh;

  enum DebugMeshType
  {
    // TODO: Support more types
    DEBUG_LINES,
  };

  struct DebugDrawData
  {
    DebugMeshType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec4 color;
    std::vector<glm::vec3> points;
    float size;
  };

  class DebugDrawSystem : public Singleton<DebugDrawSystem>
  {
  public:
    void Init();
    void Shutdown();

    void DrawLines(const std::vector<glm::vec3>& points, const glm::vec4& color);
    void DrawLines(const glm::vec3& position, const std::vector<glm::vec3>& points, const glm::vec4& color);
    void DrawPolygonOutline(const std::vector<glm::vec3>& points, const glm::vec4& color);
    void DrawRectangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
   
    void Render(int viewId);

  private:
    uint64_t _frameCount { 0 };
    std::vector<DebugDrawData> _debugDrawList;
    std::vector<Mesh*> _killList;
    Material* _debugMaterial { nullptr };
  };
}
