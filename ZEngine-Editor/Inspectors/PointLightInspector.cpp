#include "PointLightInspector.h"

#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
#include <ZEngine-Core/Component/Lighting/PointLight.h>

using namespace ZEngine;

PointLightInspector::PointLightInspector()
{
}

void PointLightInspector::RenderElement()
{
  if (GetValue() == nullptr) return;
  auto light = static_cast<PointLight*>(GetValue());

  ImGui::BeginChild(UniqueLabel("Point Light").c_str(), ImVec2(0, 120));
  {
    auto color = light->GetColor();
    float colorArray[3] = { color.x, color.y, color.z };

    if (ImGui::ColorEdit3("Color", colorArray))
    {
      light->SetColor({ colorArray[0], colorArray[1], colorArray[2] });
    }

    auto range = light->GetRange();
    if (ImGui::SliderFloat("Range", &range, 0.0f, 100.0f))
    {
      light->SetRange(range);
    }

    auto intensity = light->GetIntensity();
    if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 100.0f))
    {
      light->SetIntensity(intensity);
    }
  }
  ImGui::EndChild();
}

PointLightInspector::~PointLightInspector()
{
}
