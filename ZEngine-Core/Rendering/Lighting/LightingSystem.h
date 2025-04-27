#pragma once
#include "../../Misc/Singleton.h"
#include "../../Logging/Logger.h"
#include "../Graphics.h"
#include <glm/glm.hpp>
#include <vector>

#define MAX_LIGHTS 16

namespace ZEngine
{
  class MapManager;
  class Graphics;
  class Camera;
  class Map;

  enum LightType
  {
    DIRECTION = 0,
    POINT = 1,
    SPOT = 2,
  };

  class LightingSystem : public Singleton<LightingSystem>
  {
  public:
    void Init();
    void Shutdown();

    void Update();
    void Update(Map* map, Camera* camera);
    void ApplyUniforms();

    bool IsLightingUniform(const bgfx::UniformHandle& uniform) const;
    const std::vector<bgfx::UniformHandle*>& GetLightingUniforms() const;
  private:
    Logger _logger { "LightingSystem" };
    MapManager* _mapManager { nullptr };
    Graphics* _graphics { nullptr };

    bgfx::UniformHandle _lightTypesUniform;
    bgfx::UniformHandle _lightColorsUniform;
    bgfx::UniformHandle _lightIntensitiesUniform;
    bgfx::UniformHandle _lightPositionsUniform;
    bgfx::UniformHandle _lightDirectionsUniform;
    bgfx::UniformHandle _lightRangesUniform;
    bgfx::UniformHandle _lightCountUniform;
    bgfx::UniformHandle _cameraPosUniform;

    std::vector<bgfx::UniformHandle*> _lightingUniforms = {
      &_lightTypesUniform,
      &_lightColorsUniform,
      &_lightIntensitiesUniform,
      &_lightPositionsUniform,
      &_lightDirectionsUniform,
      &_lightRangesUniform,
      &_lightCountUniform,
      &_cameraPosUniform,
    };
    glm::vec4 _lightTypes[MAX_LIGHTS];
    glm::vec4 _lightColors[MAX_LIGHTS];
    glm::vec4 _lightIntensities[MAX_LIGHTS];
    glm::vec4 _lightPositions[MAX_LIGHTS];
    glm::vec4 _lightDirections[MAX_LIGHTS];
    glm::vec4 _lightRanges[MAX_LIGHTS];
    glm::vec4 _lightCount;
    glm::vec4 _cameraPos;
  };
}
