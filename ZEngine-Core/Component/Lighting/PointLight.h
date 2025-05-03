#pragma once

#include "Light.h"
#include "../../Utilities/JsonHelpers.h"
#include <glm/glm.hpp>

namespace ZEngine
{
  class PointLight : public Light
  {
  public:
    PointLight();
    ~PointLight() override;

    void Init() override {}
    void Update() override {}
    void Render(int viewId) override {}

    void SetColor(float r, float g, float b);
    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);
    void SetRange(float range);

    const glm::vec3& GetColor() const;
    float GetIntensity() const;
    float GetRange() const;

    static ZObject* CreateInstance(std::string name, ObjectType type);
    static ZObject* Copy(std::string name, ZObject* object);

    static ObjectType GetStaticType()
    {
      return POINT_LIGHT;
    }

#ifndef SWIG
    // Allow serialization / deserialization
    JSON_SCHEMA_BEGIN(PointLight)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (color,     GetColor,     SetColor,     glm::vec3)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (intensity, GetIntensity, SetIntensity, float)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (range,     GetRange,     SetRange,     float)
    JSON_SCHEMA_END()
#endif

  private:
    glm::vec3 _color;
    float _intensity;
    float _range;
  };
}
