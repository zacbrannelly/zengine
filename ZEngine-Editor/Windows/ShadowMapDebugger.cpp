#include "ShadowMapDebugger.h"

#include <ZEngine-Core/Rendering/FrameBuffer.h>
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Rendering/Lighting/LightingSystem.h>
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
#include "../Editor.h"

using namespace ZEngine;

ShadowMapDebugger::ShadowMapDebugger(Editor* editor) : GUIWindow("Shadow Map Debugger", 500, 500, false)
{
  _editor = editor;
  _lightingSystem = LightingSystem::GetInstance();

  _depthTexture = Graphics::GetInstance()->CreateTexture2D(
    MAX_SHADOWMAP_SIZE,
    MAX_SHADOWMAP_SIZE,
    false,
    1,
    bgfx::TextureFormat::D32F,
    BGFX_TEXTURE_BLIT_DST
  );
}

void ShadowMapDebugger::ProcessInput()
{
}

void ShadowMapDebugger::RenderInWindow()
{
  ImGui::Text("Shadow Map Debugger");

  if (!_lightingSystem)
  {
    ImGui::Text("Lighting system not initialized.");
    return;
  }

  // Bias Settings
  float depthBiasSlope = _lightingSystem->GetDepthBiasSlope();
  float depthBiasConstant = _lightingSystem->GetDepthBiasConstant();
  
  ImGui::SliderFloat("Depth Bias Slope", &depthBiasSlope, -10.0f, 10.0f);
  ImGui::SliderFloat("Depth Bias Constant", &depthBiasConstant, -10.0f, 10.0f);

  _lightingSystem->SetDepthBiasSlope(depthBiasSlope);
  _lightingSystem->SetDepthBiasConstant(depthBiasConstant);

  // Select shadow map index
  ImGui::Text("Select Shadow Map Index (0-3):");

  ImGui::InputInt("Shadow Map Index", &_selectedShadowMapIndex);
  if (_selectedShadowMapIndex < 0 || _selectedShadowMapIndex > 3)
  {
    ImGui::Text("Invalid shadow map index. Must be between 0 and 3.");
    return;
  }

  auto camera = _lightingSystem->GetCascadeShadowCamera(_selectedShadowMapIndex);
  if (camera == nullptr) return;

  auto frameBuffer = camera->GetFrameBuffer();
  auto textureArray = frameBuffer->GetTexture();
  auto layerIdx = frameBuffer->GetTextureLayer();

  // Copy the shadow map texture to the preview texture
  if (bgfx::isValid(_depthTexture) && bgfx::isValid(textureArray))
  {
    bgfx::blit(0, _depthTexture, 0, 0, 0, 0, textureArray, 0, 0, 0, layerIdx, MAX_SHADOWMAP_SIZE, MAX_SHADOWMAP_SIZE, 1);
  }

	auto viewportWidth = GetContentWidth();
  auto imageWidth = std::min(viewportWidth, MAX_SHADOWMAP_SIZE);

  if (bgfx::isValid(_depthTexture))
  {
    ImGui::Text("Shadow Map");
    ImGui::Image(ImTextureID(_depthTexture.idx), ImVec2(imageWidth, imageWidth), ImVec2(0, 0), ImVec2(1, 1));
  }
  else
  {
    ImGui::Text("No shadow map available.");
  }
}

ShadowMapDebugger::~ShadowMapDebugger()
{
  _editor = nullptr;
  _lightingSystem = nullptr;
}
