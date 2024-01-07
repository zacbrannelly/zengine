#include "AddComponentPopup.h"
#include "ComponentInfo.h"
#include "../UI/GUITextField.h"
#include "../Editor.h"

#include <ZEngine-Core/Map/Objects/ObjectType.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Scripting/CSharp/CSharpScript.h>
#include <ZEngine-Core/Component/CSharpScriptComponent.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/CSharpScriptAsset.h>


AddComponentPopup::AddComponentPopup(Editor* editor) : GUIDialog("Add Component", 300, 200, false)
{
  _editor = editor;
  _searchField = new GUITextField("Search");

  // TODO: Get the object metadata from the a static method in the class corresponding to the object type.
  RegisterComponent({ "Audio Source",        "Audio Source component",                    AUDIO_SOURCE });
  RegisterComponent({ "Camera",              "Camera component",                          CAMERA });
  RegisterComponent({ "Mesh Renderer",       "Mesh Renderer component",                   MESH_RENDERER });
  RegisterComponent({ "Rigid Body 3D",       "Rigid Body 3D component",                   RIGID_BODY_3D });
  RegisterComponent({ "Box Collider 3D",     "Box Collider 3D component",                 BOX_COLLIDER_3D });
  RegisterComponent({ "Sphere Collider 3D",  "Sphere Collider 3D component",              SPHERE_COLLIDER_3D });
  RegisterComponent({ "Capsule Collider 3D", "Capsule Collider 3D component",             CAPSULE_COLLIDER_3D });
  RegisterComponent({ "Plane Collider 3D",   "Plane Collider 3D (Static-only) component", PLANE_COLLIDER_3D });
  RegisterComponent({ "Mesh Collider 3D",    "Mesh Collider 3D (Static-only) component",  MESH_COLLIDER_3D });
}

void AddComponentPopup::RegisterComponent(ComponentInfo component)
{
  _components.push_back(component);
}

void AddComponentPopup::ProcessInput()
{
}

void AddComponentPopup::RenderInWindow()
{
  _searchField->RenderElement();

  if (ImGui::BeginListBox("###Components", ImVec2(-FLT_MIN, 0)))
  {
    // Render C++ Components
    for (auto component : _components)
    {
      if (_searchField->GetText().empty() || component.name.find(_searchField->GetText()) != std::string::npos)
      {
        if (ImGui::Selectable(component.name.c_str()))
        {
          auto newComp = Factory::CreateInstance<Component>(component.name, component.type);
          _editor->GetSelectedEntity()->AddComponent(newComp);

          ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsItemHovered())
        {
          ImGui::SetTooltip(component.description.c_str());
        }
      }
    }

    // Render C# Script Components
    auto assetManager = AssetManager::GetInstance();
    auto scriptCatalogEntries = assetManager->GetCatalog()->GetAssetsByType(CSHARP_SCRIPT_ASSET);

    for (auto scriptCatalogEntry : scriptCatalogEntries)
    {
      auto scriptAsset = assetManager->LoadAsset(scriptCatalogEntry.path, CSHARP_SCRIPT_ASSET)->Cast<CSharpScriptAsset>();
      auto scriptName = scriptAsset->GetScript()->GetClassName();

      if (_searchField->GetText().empty() || scriptName.find(_searchField->GetText()) != std::string::npos)
      {
        if (ImGui::Selectable(scriptName.c_str()))
        {
          // Create new instance of the script component and add it to the selected entity
          auto newComp = Factory::CreateInstance<CSharpScriptComponent>(scriptName, CSHARP_SCRIPT_COMPONENT);
          newComp->SetScript(scriptAsset->GetScript());
          _editor->GetSelectedEntity()->AddComponent(newComp);
          
          ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsItemHovered())
        {
          ImGui::SetTooltip("Custom C# Script Component");
        }
      }
    }
    ImGui::EndListBox();
  }
}

AddComponentPopup::~AddComponentPopup()
{
  delete _searchField;
}
