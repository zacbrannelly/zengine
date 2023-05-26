#include "../Scripting/CSharp/CSharpScript.h"
#include "CSharpScriptComponent.h"

#define INIT_METHOD_NAME "Init"
#define UPDATE_METHOD_NAME "Update"
#define RENDER_METHOD_NAME "Render"

CSharpScriptComponent::CSharpScriptComponent(std::string name) : Component(name, CSHARP_SCRIPT_COMPONENT)
{
  _script = nullptr;
  _managedInstance = nullptr;
}

void CSharpScriptComponent::Init() {
  if (!_managedInstance) {
    _managedInstance = _script->CreateManagedObject(GetOwner());
  }
  _script->InvokeMethod(_managedInstance, INIT_METHOD_NAME);
}

void CSharpScriptComponent::Update() {
  _script->InvokeMethod(_managedInstance, UPDATE_METHOD_NAME);
}

void CSharpScriptComponent::Render(int viewId) {
  _script->InvokeMethod(_managedInstance, RENDER_METHOD_NAME);
}

void CSharpScriptComponent::SetScript(CSharpScript* script) {
  _script = script;
}

CSharpScript* CSharpScriptComponent::GetScript() const {
  return _script;
}

ZObject* CSharpScriptComponent::CreateInstance(std::string name, ObjectType type)
{
  return new CSharpScriptComponent(name);
}

ZObject* CSharpScriptComponent::Copy(std::string name, ZObject* object)
{
  if (object == nullptr || object->GetType() != CSHARP_SCRIPT_COMPONENT)
    return nullptr;

  const auto newComponent = new CSharpScriptComponent(name);
  newComponent->SetScript(static_cast<CSharpScriptComponent*>(object)->GetScript());

  return newComponent;
}

CSharpScriptComponent::~CSharpScriptComponent()
{
  // TODO: Figure out how to clean up the managed instance.
}