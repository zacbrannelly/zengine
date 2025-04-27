#include "SpotLight.h"

using namespace ZEngine;

SpotLight::SpotLight() : Light("Spot Light", SPOT_LIGHT)
{
  RegisterDerivedType(SPOT_LIGHT);
  _direction = glm::vec3(0.0f, 0.0f, 1.0f);
  _color = glm::vec3(1.0f, 1.0f, 1.0f);
  _intensity = 1.0f;
  _range = 10.0f;
  _penumbraAngleRadians = M_PI / 4.0f; // 45 degrees
  _umbraAngleRadians = (65.0f * M_PI) / 180.0f; // 65 degrees
}

void SpotLight::SetDirection(float x, float y, float z)
{
  _direction = glm::vec3(x, y, z);
}

void SpotLight::SetDirection(const glm::vec3& direction)
{
  _direction = direction;
}

void SpotLight::SetColor(float r, float g, float b)
{
  _color = glm::vec3(r, g, b);
}

void SpotLight::SetColor(const glm::vec3& color)
{
  _color = color;
}

void SpotLight::SetIntensity(float intensity)
{
  _intensity = intensity;
}

void SpotLight::SetRange(float range)
{
  _range = range;
}

void SpotLight::SetPenumbraAngleRadians(float penumbra)
{
  _penumbraAngleRadians = penumbra;
}

void SpotLight::SetUmbraAngleRadians(float umbra)
{
  _umbraAngleRadians = umbra;
}

float SpotLight::GetPenumbraAngleRadians() const
{
  return _penumbraAngleRadians;
}

float SpotLight::GetUmbraAngleRadians() const
{
  return _umbraAngleRadians;
}

const glm::vec3& SpotLight::GetDirection() const
{
  return _direction;
}

const glm::vec3& SpotLight::GetColor() const
{
  return _color;
}

float SpotLight::GetIntensity() const
{
  return _intensity;
}

float SpotLight::GetRange() const
{
  return _range;
}

ZObject* SpotLight::CreateInstance(std::string name, ObjectType type)
{
  return new SpotLight();
}

ZObject* SpotLight::Copy(std::string name, ZObject* object)
{
  auto light = static_cast<SpotLight*>(object);
  auto newLight = new SpotLight();
  newLight->SetDirection(light->GetDirection());
  newLight->SetColor(light->GetColor());
  newLight->SetIntensity(light->GetIntensity());
  newLight->SetRange(light->GetRange());
  newLight->SetPenumbraAngleRadians(light->GetPenumbraAngleRadians());
  newLight->SetUmbraAngleRadians(light->GetUmbraAngleRadians());
  return newLight;
}

SpotLight::~SpotLight()
{
}
