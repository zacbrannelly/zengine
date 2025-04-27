#include "DirectionalLight.h"

using namespace ZEngine;

DirectionalLight::DirectionalLight() : Light("Directional Light", DIRECTIONAL_LIGHT)
{
  RegisterDerivedType(DIRECTIONAL_LIGHT);
  _direction = glm::vec3(0.0f, 1.0f, 0.0f);
  _color = glm::vec3(1.0f, 1.0f, 1.0f);
  _intensity = 1.0f;
}

void DirectionalLight::SetDirection(float x, float y, float z)
{
  _direction = glm::vec3(x, y, z);
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
  _direction = direction;
}

void DirectionalLight::SetColor(float r, float g, float b)
{
  _color = glm::vec3(r, g, b);
}

void DirectionalLight::SetColor(const glm::vec3& color)
{
  _color = color;
}

void DirectionalLight::SetIntensity(float intensity)
{
  _intensity = intensity;
}

const glm::vec3& DirectionalLight::GetDirection() const
{
  return _direction;
}

const glm::vec3& DirectionalLight::GetColor() const
{
  return _color;
}

float DirectionalLight::GetIntensity() const
{
  return _intensity;
}

ZObject* DirectionalLight::CreateInstance(std::string name, ObjectType type)
{
  return new DirectionalLight();
}

ZObject* DirectionalLight::Copy(std::string name, ZObject* object)
{
  auto light = static_cast<DirectionalLight*>(object);
  auto newLight = new DirectionalLight();
  newLight->SetDirection(light->GetDirection());
  newLight->SetColor(light->GetColor());
  newLight->SetIntensity(light->GetIntensity());
  return newLight;
}

DirectionalLight::~DirectionalLight()
{
}
