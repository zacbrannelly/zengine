#include "../Scripting/CSharp/CSharpScript.h"
#include "CSharpScriptComponent.h"

#define INIT_METHOD_NAME "Init"
#define UPDATE_METHOD_NAME "Update"
#define RENDER_METHOD_NAME "Render"

using namespace ZEngine;

CSharpScriptComponent::CSharpScriptComponent() : Component("CSharpScriptComponent", CSHARP_SCRIPT_COMPONENT)
{
  RegisterDerivedType(CSHARP_SCRIPT_COMPONENT);
  _script = nullptr;
  _managedInstance = nullptr;
}

#ifndef __EMSCRIPTEN__
void CSharpScriptComponent::Init() {
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
  SetName(script->GetClassName());
  SetManagedInstance(_script->CreateManagedObject(this));
}
#else
void CSharpScriptComponent::Init() {
  // TODO: Support scripting in emscripten.
}

void CSharpScriptComponent::Update() {
  // TODO: Support scripting in emscripten.
}

void CSharpScriptComponent::Render(int viewId) {
  // TODO: Support scripting in emscripten.
}

void CSharpScriptComponent::SetScript(CSharpScript* script) {
  _script = script;
  // TODO: Support scripting in emscripten.
}
#endif

CSharpScript* CSharpScriptComponent::GetScript() const {
  return _script;
}

void CSharpScriptComponent::SetManagedInstance(void* instance) {
  _managedInstance = instance;
}

void* CSharpScriptComponent::GetManagedInstance() const {
  return _managedInstance;
}

ZObject* CSharpScriptComponent::CreateInstance(std::string name, ObjectType type)
{
  return new CSharpScriptComponent();
}

ZObject* CSharpScriptComponent::Copy(std::string name, ZObject* object)
{
  if (object == nullptr || object->GetType() != CSHARP_SCRIPT_COMPONENT)
    return nullptr;

  const auto newComponent = new CSharpScriptComponent();
  newComponent->SetName(name);
  newComponent->SetScript(static_cast<CSharpScriptComponent*>(object)->GetScript());

  return newComponent;
}

CSharpScriptComponent::~CSharpScriptComponent()
{
  // TODO: Figure out how to clean up the managed instance.
}
