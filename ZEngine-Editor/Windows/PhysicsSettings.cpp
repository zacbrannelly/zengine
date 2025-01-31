#include "PhysicsSettings.h"
#include "../Editor.h"
#include <ZEngine-Core/Map/Map.h>

using namespace ZEngine;

PhysicsSettings::PhysicsSettings(Editor* editor) : GUIWindow("Physics Settings", 400, 300, false), _editor(editor)
{
}

void PhysicsSettings::ProcessInput()
{
}

void PhysicsSettings::RenderInWindow()
{
  ImGui::Text("Physics Settings");
  ImGui::Separator();
  
  auto map = _editor->GetSelectedMap();

  if (map == nullptr)
    return;

  auto physicsSettings = map->GetPhysicsSceneDescription();

  glm::vec3 gravity = physicsSettings.gravity;
  if (ImGui::InputFloat3("Gravity", &gravity.x))
  {
    physicsSettings.gravity = gravity;
    map->SetPhysicsSceneDescription(physicsSettings);
  }
}

PhysicsSettings::~PhysicsSettings()
{
}
