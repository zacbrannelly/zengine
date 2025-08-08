#pragma once
#include "../../Misc/Singleton.h"
#include "../../Logging/Logger.h"
#include "../Graphics.h"
#include <glm/glm.hpp>
#include <vector>

#define MAX_LIGHTS 16
#define MAX_CASCADES 4
#define MAX_SHADOWMAP_SIZE 1024

namespace ZEngine
{
  class MapManager;
  class Graphics;
  class Camera;
  class Map;
  class DirectionalLight;

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
    void Update(Map* map, Camera* camera, int shadowBaseViewId);
    void ApplyUniforms();

    bool IsLightingUniform(const bgfx::UniformHandle& uniform) const;
    const std::vector<bgfx::UniformHandle*>& GetLightingUniforms() const;

    Camera* GetCascadeShadowCamera(int index) const;

    void SetDepthBiasSlope(float slope) { _depthBiasSlope.x = slope; }
    void SetDepthBiasConstant(float constant) { _depthBiasConstant.x = constant; }
    float GetDepthBiasSlope() const { return _depthBiasSlope.x; }
    float GetDepthBiasConstant() const { return _depthBiasConstant.x; }
  
  private:
    void UpdateCascadedShadowMap(Map* map, Camera* camera, DirectionalLight* light, uint8_t lightIdx);

    Logger _logger { "LightingSystem" };
    MapManager* _mapManager { nullptr };
    Graphics* _graphics { nullptr };
    Camera* _cascadeShadowCameras[MAX_CASCADES];

    bgfx::UniformHandle _lightTypesUniform;
    bgfx::UniformHandle _lightColorsUniform;
    bgfx::UniformHandle _lightIntensitiesUniform;
    bgfx::UniformHandle _lightPositionsUniform;
    bgfx::UniformHandle _lightDirectionsUniform;
    bgfx::UniformHandle _lightRangesUniform;
    bgfx::UniformHandle _lightPrenumbraAndUmbraUniform;
    bgfx::UniformHandle _lightCountUniform;
    bgfx::UniformHandle _cameraPosUniform;
    bgfx::UniformHandle _cascadeDepthArrayUniform;
    bgfx::UniformHandle _lightViewProjectionMatrixArrayUniform;
    bgfx::UniformHandle _depthBiasSlopeUniform;
    bgfx::UniformHandle _depthBiasConstantUniform;
    bgfx::UniformHandle _cascadeShadowMapArrayUniform;
    bgfx::TextureHandle _cascadeShadowMapArray;

    std::vector<bgfx::UniformHandle*> _lightingUniforms = {
      &_lightTypesUniform,
      &_lightColorsUniform,
      &_lightIntensitiesUniform,
      &_lightPositionsUniform,
      &_lightDirectionsUniform,
      &_lightRangesUniform,
      &_lightPrenumbraAndUmbraUniform,
      &_lightCountUniform,
      &_cameraPosUniform,
      &_cascadeDepthArrayUniform,
      &_lightViewProjectionMatrixArrayUniform,
      &_cascadeShadowMapArrayUniform,
      &_depthBiasSlopeUniform,
      &_depthBiasConstantUniform
    };
    glm::vec4 _lightTypes[MAX_LIGHTS];
    glm::vec4 _lightColors[MAX_LIGHTS];
    glm::vec4 _lightIntensities[MAX_LIGHTS];
    glm::vec4 _lightPositions[MAX_LIGHTS];
    glm::vec4 _lightDirections[MAX_LIGHTS];
    glm::vec4 _lightRanges[MAX_LIGHTS];
    glm::vec4 _lightPrenumbraAndUmbra[MAX_LIGHTS];
    glm::vec4 _lightCount;
    glm::vec4 _cameraPos;
    glm::vec4 _depthBiasSlope;
    glm::vec4 _depthBiasConstant;

    glm::mat4 _lightViewProjectionMatrixArray[MAX_CASCADES];
    glm::vec4 _cascadeDepthArray[MAX_CASCADES];
  };
}
