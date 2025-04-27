#include "SpotLightInspector.h"

#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
#include <ZEngine-Core/Component/Lighting/SpotLight.h>

using namespace ZEngine;

SpotLightInspector::SpotLightInspector()
{
}

void SpotLightInspector::RenderElement()
{
  if (GetValue() == nullptr) return;
  auto light = static_cast<SpotLight*>(GetValue());

  ImGui::BeginChild(UniqueLabel("Spot Light").c_str(), ImVec2(0, 140));
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

    float penumbra = light->GetPenumbraAngleRadians() * 180.0f / M_PI;
    if (ImGui::SliderFloat("Penumbra Angle", &penumbra, 0.0f, 180.0f))
    {
      light->SetPenumbraAngleRadians(penumbra * M_PI / 180.0f);
    }

    float umbra = light->GetUmbraAngleRadians() * 180.0f / M_PI;
    if (ImGui::SliderFloat("Umbra Angle", &umbra, 0.0f, 180.0f))
    {
      light->SetUmbraAngleRadians(umbra * M_PI / 180.0f);
    }
  }
  ImGui::EndChild();
}

SpotLightInspector::~SpotLightInspector()
{
}
