#include "DirectionalLightInspector.h"

#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
#include <ZEngine-Core/Component/Lighting/DirectionalLight.h>

using namespace ZEngine;

DirectionalLightInspector::DirectionalLightInspector()
{
}

void DirectionalLightInspector::RenderElement()
{
  if (GetValue() == nullptr) return;
  auto directionalLight = static_cast<DirectionalLight*>(GetValue());

  ImGui::BeginChild(UniqueLabel("Directional Light").c_str(), ImVec2(0, 80));
  {
    auto color = directionalLight->GetColor();
    float colorArray[3] = { color.x, color.y, color.z };

    if (ImGui::ColorEdit3("Color", colorArray))
    {
      directionalLight->SetColor({ colorArray[0], colorArray[1], colorArray[2] });
    }

    auto intensity = directionalLight->GetIntensity();
    if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 100.0f))
    {
      directionalLight->SetIntensity(intensity);
    }
  }
  ImGui::EndChild();
}

DirectionalLightInspector::~DirectionalLightInspector()
{
}
