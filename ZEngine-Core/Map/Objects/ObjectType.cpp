#include "ObjectType.h"
#include <stdexcept>
#include <unordered_map>

static std::unordered_map<std::string, ObjectType> _stringToObjectTypeMap;
static std::unordered_map<ObjectType, std::string> _objectTypeToStringMap;

#define ADD_OBJECT_TYPE(type) \
  _stringToObjectTypeMap[#type] = ObjectType::type; \
  _objectTypeToStringMap[ObjectType::type] = #type;

void RegisterObjectTypes() {
  ADD_OBJECT_TYPE(SHADER)
  ADD_OBJECT_TYPE(MATERIAL)
  ADD_OBJECT_TYPE(ENTITY)
  ADD_OBJECT_TYPE(MAP)
  ADD_OBJECT_TYPE(COMPONENT)
  ADD_OBJECT_TYPE(CAMERA)
  ADD_OBJECT_TYPE(TRANSFORM)
  ADD_OBJECT_TYPE(MESH)
  ADD_OBJECT_TYPE(MESH_RENDERER)
  ADD_OBJECT_TYPE(ASSET)
  ADD_OBJECT_TYPE(TEXTURE_ASSET)
  ADD_OBJECT_TYPE(SHADER_ASSET)
  ADD_OBJECT_TYPE(MATERIAL_ASSET)
  ADD_OBJECT_TYPE(MODEL_ASSET)
  ADD_OBJECT_TYPE(MAP_ASSET)
  ADD_OBJECT_TYPE(AUDIO_ASSET)
  ADD_OBJECT_TYPE(AUDIO_SOURCE)
  ADD_OBJECT_TYPE(CSHARP_SCRIPT_COMPONENT)
  ADD_OBJECT_TYPE(CSHARP_SCRIPT)
  ADD_OBJECT_TYPE(CSHARP_SCRIPT_ASSET)
  ADD_OBJECT_TYPE(RIGID_BODY_3D)
  ADD_OBJECT_TYPE(COLLIDER_3D)
  ADD_OBJECT_TYPE(BOX_COLLIDER_3D)
}

std::string ObjectTypeToString(ObjectType type) {
  if (_objectTypeToStringMap.size() == 0) {
    RegisterObjectTypes();
  }

  const auto result = _objectTypeToStringMap.find(type);
  if (result == _objectTypeToStringMap.end()) {
    throw std::runtime_error("Could not convert object type to string!");
  }
  return result->second;
}

ObjectType StringToObjectType(std::string type) {
  if (_objectTypeToStringMap.size() == 0) {
    RegisterObjectTypes();
  }

  const auto result = _stringToObjectTypeMap.find(type);
  if (result == _stringToObjectTypeMap.end()) {
    throw std::runtime_error("Could not convert string to object type!");
  }
  return result->second;
}
