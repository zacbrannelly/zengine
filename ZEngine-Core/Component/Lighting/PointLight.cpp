#include "PointLight.h"

using namespace ZEngine;

PointLight::PointLight() : Light("Point Light", POINT_LIGHT)
{
  RegisterDerivedType(POINT_LIGHT);
  _color = glm::vec3(1.0f, 1.0f, 1.0f);
  _intensity = 1.0f;
  _range = 10.0f;
}

void PointLight::SetColor(float r, float g, float b)
{
  _color = glm::vec3(r, g, b);
}

void PointLight::SetColor(const glm::vec3& color)
{
  _color = color;
}

void PointLight::SetIntensity(float intensity)
{
  _intensity = intensity;
}

void PointLight::SetRange(float range)
{
  _range = range;
}

const glm::vec3& PointLight::GetColor() const
{
  return _color;
}

float PointLight::GetIntensity() const
{
  return _intensity;
}

float PointLight::GetRange() const
{
  return _range;
}

ZObject* PointLight::CreateInstance(std::string name, ObjectType type)
{
  return new PointLight();
}

ZObject* PointLight::Copy(std::string name, ZObject* object)
{
  auto light = static_cast<PointLight*>(object);
  auto newLight = new PointLight();
  newLight->SetColor(light->GetColor());
  newLight->SetIntensity(light->GetIntensity());
  newLight->SetRange(light->GetRange());
  return newLight;
}

PointLight::~PointLight()
{
}
