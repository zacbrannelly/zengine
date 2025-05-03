#include "Light.h"
#include "../../Utilities/JsonHelpers.h"
#include <glm/glm.hpp>

namespace ZEngine
{
  class DirectionalLight : public Light
  {
  public:
    DirectionalLight();
    ~DirectionalLight() override;

    void Init() override {}
    void Update() override {}
    void Render(int viewId) override {}

    void SetDirection(float x, float y, float z);
    void SetDirection(const glm::vec3& direction);
    void SetColor(float r, float g, float b);
    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);

    const glm::vec3& GetDirection() const;
    const glm::vec3& GetColor() const;
    float GetIntensity() const;

    static ZObject* CreateInstance(std::string name, ObjectType type);
    static ZObject* Copy(std::string name, ZObject* object);

    static ObjectType GetStaticType()
    {
      return DIRECTIONAL_LIGHT;
    }

#ifndef SWIG
    // Allow serialization / deserialization
    JSON_SCHEMA_BEGIN(DirectionalLight)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (direction, GetDirection, SetDirection, glm::vec3)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (color,     GetColor,     SetColor,     glm::vec3)
      JSON_MAP_TO_GETTER_SETTER_OPTIONAL (intensity, GetIntensity, SetIntensity, float)
    JSON_SCHEMA_END()
#endif
  private:
    glm::vec3 _direction;
    glm::vec3 _color;
    float _intensity;
  };
}
