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
#include "../../Math/AABB.h"
#include "../Shader.h"
#include "../StandardShaders.h"
#include "../FrameBuffer.h"
#include "../DebugDrawSystem.h"

#include <iostream>
#include <algorithm>

using namespace ZEngine;

namespace ZEngine
{
  class ShadowCamera : public Camera
  {
  public:
    ShadowCamera(uint32_t size, bgfx::TextureHandle depthTextureArray, int layerIdx)
    {
      // Render to a specific layer of the provided texture array
      auto frameBuffer = new FrameBuffer();
      frameBuffer->Build(depthTextureArray, layerIdx);
      SetFrameBuffer(frameBuffer);

      SetViewport(0, 0, size, size);
      SetProjectionMode(Camera::ORTHOGRAPHIC);
    }

    void SetViewMatrix(const glm::mat4& viewMatrix)
    {
      _viewMatrix = viewMatrix;
    }

    glm::mat4 GetViewMatrix() const override
    {
      return _viewMatrix;
    }
  private:
    glm::mat4 _viewMatrix;
  };
}

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

  // Shadow map
  _cascadeShadowMapArray = _graphics->CreateTexture2D(
    MAX_SHADOWMAP_SIZE,
    MAX_SHADOWMAP_SIZE,
    false,
    MAX_CASCADES,
    bgfx::TextureFormat::D32F,
    BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
  );
  for (int i = 0; i < MAX_CASCADES; i++)
  {
    _cascadeShadowCameras[i] = new ShadowCamera(MAX_SHADOWMAP_SIZE, _cascadeShadowMapArray, i);
  }
  _cascadeDepthArrayUniform =              _graphics->CreateUniform("u_cascadeDepths", bgfx::UniformType::Vec4, MAX_CASCADES);
  _lightViewProjectionMatrixArrayUniform = _graphics->CreateUniform("u_lightViewProjections", bgfx::UniformType::Mat4, MAX_CASCADES);
  _cascadeShadowMapArrayUniform =          _graphics->CreateUniform("s_cascadeShadowMap", bgfx::UniformType::Sampler, MAX_CASCADES);
  _depthBiasSlopeUniform =                 _graphics->CreateUniform("u_depthBiasSlope", bgfx::UniformType::Vec4, 1);
  _depthBiasConstantUniform =              _graphics->CreateUniform("u_depthBiasConstant", bgfx::UniformType::Vec4, 1);

  _depthBiasConstant = glm::vec4(0.005f);
  _depthBiasSlope = glm::vec4(0.02f);

  _logger.LogInfo("Lighting system initialized.");
}

void LightingSystem::Update()
{
  // Get current map from the map manager.
  auto currentMap = _mapManager->GetCurrentMap();
  if (!currentMap) return;

  // Get the first camera from the current map.
  auto camera = currentMap->GetCameras().empty() 
    ? nullptr 
    : currentMap->GetCameras()[0];

  // Update the lighting system.
  Update(currentMap, camera);
}

void LightingSystem::Update(Map* currentMap, Camera* camera)
{
  Update(currentMap, camera, ZENGINE_SHADOW_PASS_VIEW_ID(0));
}

void LightingSystem::Update(Map* currentMap, Camera* camera, int shadowBaseViewId)
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

        // Order the draw calls for the shadow cameras correctly.
        for (int j = 0; j < MAX_CASCADES; ++j)
        {
          auto shadowCamera = _cascadeShadowCameras[j];
          shadowCamera->SetViewId(shadowBaseViewId + j);
        }

        // Update the cascaded shadow map for the directional light (if any).
        UpdateCascadedShadowMap(currentMap, camera, dirLight, totalLightCount);
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

void LightingSystem::UpdateCascadedShadowMap(Map* map, Camera* camera, DirectionalLight* light, uint8_t lightIdx)
{
  if (map == nullptr || camera == nullptr || light == nullptr) return;

  // Calculate the Z-planes for the cascaded shadow map.
  float lambda = 0.75f;
  float near = camera->GetNear();
  float far = camera->GetFar();

  float cameraRatio = far / near;
  float depthRange = far - near;

  float cascadeSplits[MAX_CASCADES * 2];
  cascadeSplits[0] = near;
  for (int i = 2; i < MAX_CASCADES * 2; i += 2)
  {
    // Linear-log blending for the cascade distances
    float cascadeRatio = static_cast<float>(i / 2) / static_cast<float>(MAX_CASCADES);
    float distance = (
      lambda * (near * pow(cameraRatio, cascadeRatio)) + 
      (1.0f - lambda) * (near + depthRange * cascadeRatio)
    );

    // Near for the current cascade
    cascadeSplits[i] = distance;

    // Far for the previous cascade
    cascadeSplits[i - 1] = distance;
  }

  // Set the last cascade far plane to the camera far plane
  cascadeSplits[MAX_CASCADES * 2 - 1] = far;

  for (int i = 0; i < MAX_CASCADES; ++i)
  {
    _cascadeDepthArray[i] = glm::vec4(0.0f, 0.0f, cascadeSplits[i * 2], 1.0f);
  }

  float tanFov = tanf(glm::radians(camera->GetFieldOfView() / 2.0f));

  for (int i = 0; i < MAX_CASCADES; ++i)
  {
    float cascadeSplitNear = cascadeSplits[i * 2];
    float cascadeSplitFar = cascadeSplits[i * 2 + 1];

    float nh = cascadeSplitNear * tanFov;
    float nw = nh * camera->GetAspectRatio();

    float fh = cascadeSplitFar * tanFov;
    float fw = fh * camera->GetAspectRatio();

    const glm::vec4 frustumCornersInViewSpace[8] =
    {
      { -nw,  nh, cascadeSplitNear, 1 },
      {  nw,  nh, cascadeSplitNear, 1 },
      {  nw, -nh, cascadeSplitNear, 1 },
      { -nw, -nh, cascadeSplitNear, 1 },
      { -fw,  fh, cascadeSplitFar,  1 },
      {  fw,  fh, cascadeSplitFar,  1 },
      {  fw, -fh, cascadeSplitFar,  1 },
      { -fw, -fh, cascadeSplitFar,  1 },
    };
    
    // Transform the frustum corners from view space to world space.
    glm::vec3 frustumCornersInWorldSpace[8];
    glm::mat4 invViewMatrix = glm::inverse(camera->GetViewMatrix());
    for (int j = 0; j < 8; ++j)
    {
      glm::vec4 corner = invViewMatrix * frustumCornersInViewSpace[j];
      frustumCornersInWorldSpace[j] = glm::vec3(corner);
    }

    // Transform the frustum corners to light space
    auto lightDir = _lightDirections[lightIdx];
    glm::mat4 lightViewMatrix = glm::lookAt(
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(lightDir),
      glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::vec3 frustumCornersInLightSpace[8];
    for (int j = 0; j < 8; ++j)
    {
      frustumCornersInLightSpace[j] = lightViewMatrix * glm::vec4(frustumCornersInWorldSpace[j], 1.0f);
    }

    // Calculate the AABB for the frustum corners (in light space)
    AABB frustumAABB;
    frustumAABB.SetFromVertices(frustumCornersInLightSpace, 8);

    auto min = frustumAABB.GetMin();
    auto max = frustumAABB.GetMax();

    // Set up the shadow camera
    auto shadowCamera = static_cast<ShadowCamera*>(_cascadeShadowCameras[i]);
    shadowCamera->SetOrtho(min.x, max.x, min.y, max.y);
    shadowCamera->SetNear(min.z);
    shadowCamera->SetFar(max.z);
    shadowCamera->SetViewMatrix(lightViewMatrix);

    // Provide the lighting shader with the light's VP matrix.
    _lightViewProjectionMatrixArray[i] = shadowCamera->GetViewProjectionMatrix();

    // Render the shadow map.
    auto shader = StandardShaders::GetShadowMapShader();
    if (shader == nullptr)
    {
      _logger.LogError("UpdateCascadedShadowMap: Shadow map shader is null.");
      return;
    }
    
    Pass pass = shader->GetPasses()[0];
    pass.state = 0
      | BGFX_STATE_WRITE_Z
      | BGFX_STATE_DEPTH_TEST_LESS
      | BGFX_STATE_CULL_CW
      | BGFX_STATE_MSAA;
    shadowCamera->Render(-1);
    map->RenderWorldToShadowMap(shadowCamera->GetViewId(), pass);
  }
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

  // Set the shadow map uniforms
  _graphics->SetTexture(0, _cascadeShadowMapArrayUniform, _cascadeShadowMapArray);
  _graphics->SetUniform(_cascadeDepthArrayUniform, _cascadeDepthArray, MAX_CASCADES);
  _graphics->SetUniform(_lightViewProjectionMatrixArrayUniform, _lightViewProjectionMatrixArray, MAX_CASCADES);
  _graphics->SetUniform(_depthBiasSlopeUniform, &_depthBiasSlope, 1);
  _graphics->SetUniform(_depthBiasConstantUniform, &_depthBiasConstant, 1);
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

Camera* LightingSystem::GetCascadeShadowCamera(int index) const
{
  if (index < 0 || index >= MAX_CASCADES)
  {
    _logger.LogError("GetCascadeShadowCamera: Index out of bounds.");
    return nullptr;
  }
  return _cascadeShadowCameras[index];
}

void LightingSystem::Shutdown()
{
  for (int i = 0; i < MAX_CASCADES; i++)
  {
    delete _cascadeShadowCameras[i];
  }

  _graphics->DestroyUniform(_cameraPosUniform);
  _graphics->DestroyUniform(_lightCountUniform);
  _graphics->DestroyUniform(_lightTypesUniform);
  _graphics->DestroyUniform(_lightColorsUniform);
  _graphics->DestroyUniform(_lightIntensitiesUniform);
  _graphics->DestroyUniform(_lightPositionsUniform);
  _graphics->DestroyUniform(_lightRangesUniform);
  _graphics->DestroyUniform(_lightPrenumbraAndUmbraUniform);
  _graphics->DestroyUniform(_lightDirectionsUniform);
  _graphics->DestroyUniform(_cascadeDepthArrayUniform);
  _graphics->DestroyUniform(_lightViewProjectionMatrixArrayUniform);
  _graphics->DestroyUniform(_depthBiasSlopeUniform);
  _graphics->DestroyUniform(_depthBiasConstantUniform);
  _graphics->DestroyUniform(_cascadeShadowMapArrayUniform);
  _graphics->DestroyTexture(_cascadeShadowMapArray);

  _logger.LogInfo("Lighting system shut down.");
}
