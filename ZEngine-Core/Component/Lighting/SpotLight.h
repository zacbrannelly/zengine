#pragma once 

#include "Light.h"
#include "../../Utilities/JsonHelpers.h"
#include <glm/glm.hpp>

namespace ZEngine
{
  class SpotLight : public Light
  {
  public:
    SpotLight();
    ~SpotLight() override;

    void Init() override {}
    void Update() override {}
    void Render(int viewId) override {}

    void SetDirection(float x, float y, float z);
    void SetDirection(const glm::vec3& direction);
    void SetColor(float r, float g, float b);
    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);
    void SetRange(float range);
    void SetPenumbraAngleRadians(float penumbra);
    void SetUmbraAngleRadians(float umbra);

    const glm::vec3& GetDirection() const;
    const glm::vec3& GetColor() const;
    float GetIntensity() const;
    float GetRange() const;
    float GetPenumbraAngleRadians() const;
    float GetUmbraAngleRadians() const;

    static ZObject* CreateInstance(std::string name, ObjectType type);
    static ZObject* Copy(std::string name, ZObject* object);

    static ObjectType GetStaticType()
    {
      return SPOT_LIGHT;
    }

#ifndef SWIG
    // Allow serialization / deserialization
    JSON_SCHEMA_BEGIN(SpotLight)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (direction, GetDirection,            SetDirection,            glm::vec3)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (color,     GetColor,                SetColor,                glm::vec3)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (intensity, GetIntensity,            SetIntensity,            float)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (range,     GetRange,                SetRange,                float)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (penumbra,  GetPenumbraAngleRadians, SetPenumbraAngleRadians, float)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (umbra,     GetUmbraAngleRadians,    SetUmbraAngleRadians,    float)
    JSON_SCHEMA_END()
#endif
  private:
    glm::vec3 _direction;
    glm::vec3 _color;
    float _intensity;
    float _range;
    float _penumbraAngleRadians;
    float _umbraAngleRadians;
  };
}
