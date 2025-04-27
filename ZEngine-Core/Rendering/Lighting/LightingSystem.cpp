#include "LightingSystem.h"
#include "../../Map/MapManager.h"
#include "../../Map/Map.h"
#include "../../Map/Objects/Entity.h"
#include "../../Component/Camera.h"
#include "../../Component/Transform.h"
#include "../../Component/Lighting/Light.h"
#include "../../Component/Lighting/DirectionalLight.h"
#include "../../Component/Lighting/PointLight.h"
#include "../../Component/Lighting/SpotLight.h"

#include <algorithm>

using namespace ZEngine;

void LightingSystem::Init()
{
  _mapManager = MapManager::GetInstance();
  if (!_mapManager)
  {
    _logger.LogError("Init: MapManager instance is null. Lighting system cannot be initialized.");
    return;
  }

  _graphics = Graphics::GetInstance();
  if (!_graphics)
  {
    _logger.LogError("Init: Graphics instance is null. Lighting system cannot be initialized.");
    return;
  }

  // General light uniforms
  _cameraPosUniform =        _graphics->CreateUniform("u_cameraPos", bgfx::UniformType::Vec4, 1);
  _lightTypesUniform =       _graphics->CreateUniform("u_lightTypes", bgfx::UniformType::Vec4, MAX_LIGHTS);
  _lightCountUniform =       _graphics->CreateUniform("u_lightCount", bgfx::UniformType::Vec4, 1);
  _lightColorsUniform =      _graphics->CreateUniform("u_lightColors", bgfx::UniformType::Vec4, MAX_LIGHTS);
  _lightIntensitiesUniform = _graphics->CreateUniform("u_lightIntensities", bgfx::UniformType::Vec4, MAX_LIGHTS);

  // Shared light uniforms
  _lightPositionsUniform =  _graphics->CreateUniform("u_lightPositions", bgfx::UniformType::Vec4, MAX_LIGHTS);
  _lightRangesUniform =     _graphics->CreateUniform("u_lightRanges", bgfx::UniformType::Vec4, MAX_LIGHTS);
  _lightDirectionsUniform = _graphics->CreateUniform("u_lightDirections", bgfx::UniformType::Vec4, MAX_LIGHTS);

  // Spot light uniforms
  _lightPrenumbraAndUmbraUniform = _graphics->CreateUniform("u_lightPrenumbraAndUmbra", bgfx::UniformType::Vec4, MAX_LIGHTS);

  _logger.LogInfo("Lighting system initialized.");
}

void LightingSystem::Update()
{
  // Get current map from the map manager.
  auto currentMap = _mapManager->GetCurrentMap();
  if (!currentMap) return;

  // Get the first camera from the current map.
  auto camera = currentMap->GetCameras().empty() ? nullptr : currentMap->GetCameras()[0];

  // Update the lighting system.
  Update(currentMap, camera);
}

void LightingSystem::Update(Map* currentMap, Camera* camera)
{
  if (currentMap == nullptr)
  {
    _logger.LogError("Update: Current map is null. Cannot update lighting system.");
    return;
  }

  if (camera != nullptr)
  {
    _cameraPos = glm::vec4(camera->GetOwner()->GetTransform()->GetWorldPosition(), 1.0f);
  }

  uint16_t totalLightCount = 0;

  for (const auto& entity : currentMap->GetEntities())
  {
    auto lights = entity->GetComponentsByType<Light>();
    if (lights.empty()) continue;

    for (int i = 0; i < lights.size(); ++i)
    {
      auto light = lights[i];
      if (light == nullptr) continue;
      if (totalLightCount + 1 >= MAX_LIGHTS) break;

      if (light->IsDerivedType(DIRECTIONAL_LIGHT) )
      {
        auto dirLight = static_cast<DirectionalLight*>(light);
        _lightTypes[totalLightCount] = glm::vec4(DIRECTION, 0.0f, 0.0f, 0.0f);
        _lightColors[totalLightCount] = glm::vec4(dirLight->GetColor(), 1.0f);
        _lightIntensities[totalLightCount] = glm::vec4(dirLight->GetIntensity(), 0.0f, 0.0f, 0.0f);
        _lightDirections[totalLightCount] = entity->GetTransform()->GetWorldRotationQuaternion() * glm::vec4(dirLight->GetDirection(), 0.0f);
      }
      else if (light->IsDerivedType(POINT_LIGHT))
      {
        auto pointLight = static_cast<PointLight*>(light);
        _lightTypes[totalLightCount] = glm::vec4(POINT, 0.0f, 0.0f, 0.0f);
        _lightColors[totalLightCount] = glm::vec4(pointLight->GetColor(), 1.0f);
        _lightIntensities[totalLightCount] = glm::vec4(pointLight->GetIntensity(), 0.0f, 0.0f, 0.0f);
        _lightRanges[totalLightCount] = glm::vec4(pointLight->GetRange(), 0.0f, 0.0f, 0.0f);
        _lightPositions[totalLightCount] = glm::vec4(entity->GetTransform()->GetWorldPosition(), 1.0f);
      }
      else if (light->IsDerivedType(SPOT_LIGHT))
      {
        auto spotLight = static_cast<SpotLight*>(light);
        _lightTypes[totalLightCount] = glm::vec4(SPOT, 0.0f, 0.0f, 0.0f);
        _lightColors[totalLightCount] = glm::vec4(spotLight->GetColor(), 1.0f);
        _lightIntensities[totalLightCount] = glm::vec4(spotLight->GetIntensity(), 0.0f, 0.0f, 0.0f);
        _lightRanges[totalLightCount] = glm::vec4(spotLight->GetRange(), 0.0f, 0.0f, 0.0f);
        _lightPositions[totalLightCount] = glm::vec4(entity->GetTransform()->GetWorldPosition(), 1.0f);
        _lightDirections[totalLightCount] = entity->GetTransform()->GetWorldRotationQuaternion() * glm::vec4(spotLight->GetDirection(), 0.0f);
        _lightPrenumbraAndUmbra[totalLightCount] = glm::vec4(spotLight->GetPenumbraAngleRadians(), spotLight->GetUmbraAngleRadians(), 0.0f, 0.0f);
      }
      else
      {
        continue;
      }

      totalLightCount++;
    }
  }
  
  // Set the light count
  _lightCount = glm::vec4(totalLightCount, 0.0f, 0.0f, 0.0f);
}

void LightingSystem::ApplyUniforms()
{
  auto const numLights = static_cast<uint16_t>(_lightCount.x);
  _graphics->SetUniform(_cameraPosUniform,              &_cameraPos,             1);
  _graphics->SetUniform(_lightCountUniform,             &_lightCount,            1);
  _graphics->SetUniform(_lightTypesUniform,             _lightTypes,             numLights);
  _graphics->SetUniform(_lightColorsUniform,            _lightColors,            numLights);
  _graphics->SetUniform(_lightIntensitiesUniform,       _lightIntensities,       numLights);
  _graphics->SetUniform(_lightPositionsUniform,         _lightPositions,         numLights);
  _graphics->SetUniform(_lightRangesUniform,            _lightRanges,            numLights);
  _graphics->SetUniform(_lightPrenumbraAndUmbraUniform, _lightPrenumbraAndUmbra, numLights);
  _graphics->SetUniform(_lightDirectionsUniform,        _lightDirections,        numLights);
}

bool LightingSystem::IsLightingUniform(const bgfx::UniformHandle& uniform) const
{
  for (const auto& lightingUniform : _lightingUniforms)
  {
    if (lightingUniform->idx == uniform.idx)
    {
      return true;
    }
  }

  return false;
}

const std::vector<bgfx::UniformHandle*>& LightingSystem::GetLightingUniforms() const
{
  return _lightingUniforms;
}

void LightingSystem::Shutdown()
{
  _graphics->DestroyUniform(_cameraPosUniform);
  _graphics->DestroyUniform(_lightCountUniform);
  _graphics->DestroyUniform(_lightTypesUniform);
  _graphics->DestroyUniform(_lightColorsUniform);
  _graphics->DestroyUniform(_lightIntensitiesUniform);
  _graphics->DestroyUniform(_lightPositionsUniform);
  _graphics->DestroyUniform(_lightRangesUniform);
  _graphics->DestroyUniform(_lightPrenumbraAndUmbraUniform);
  _graphics->DestroyUniform(_lightDirectionsUniform);
  _logger.LogInfo("Lighting system shut down.");
}
  