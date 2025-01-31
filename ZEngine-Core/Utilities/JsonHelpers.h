#pragma once

/**
 * Modification of the nlohmann JSON library to allow for serialization of classes using underscores in their member names.
 * Original code can be found at: https://github.com/nlohmann/json/blob/0457de21cffb298c22b629e538036bfeb96130b7/single_include/nlohmann/json.hpp#L2748
 */

#include <nlohmann/json.hpp>
#include <string>
#include <uuid.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Assets/AssetManager.h"
#include "../Assets/AssetCatalog.h"
#include "../Misc/Factory.h"

/**
 * 
 * Custom macro DSL for JSON schema declaration for serialization and deserialization of classes.
 * 
 * JSON_SCHEMA_BEGIN()
 * 
 *  JSON_MAP_TO_MEMBER             ("name", _name);
 *  JSON_MAP_TO_GETTER_SETTER      ("name", GetterFunc, SetterFunc, std::string);
 *  JSON_MAP_TO_SETTER             ("name", SetterFunc, std::string);
 *  JSON_MAP_TO_SETTER_OPTIONAL    ("name", SetterFunc, std::string);
 * 
 * JSON_SCHEMA_END()
 */

#define JSON_SCHEMA_BEGIN(Type) \
  friend void to_json(nlohmann::json& out, const Type& in) \
  { \
    JsonSerialization(&in, &out, nullptr, nullptr); \
  } \
  friend void from_json(const nlohmann::json& in, Type& out) \
  { \
    JsonSerialization(nullptr, nullptr, &in, &out); \
  } \
  \
  friend void JsonSerialization(const Type* exportIn, nlohmann::json* exportOut, const nlohmann::json* parseIn, Type* parseOut) { \
    bool isParsing = parseIn != nullptr && parseOut != nullptr; \

#define JSON_SCHEMA_END() \
  }

#define JSON_MAP_TO_MEMBER(member, memberName) \
  if (isParsing) \
  { \
    (*parseIn).at(#member).get_to(parseOut->memberName); \
  } \
  else \
  { \
    (*exportOut)[#member] = exportIn->memberName; \
  }

#define JSON_MAP_TO_MEMBER_OPTIONAL(member, memberName) \
  if (isParsing && (*parseIn).contains(#member)) \
  { \
    (*parseIn).at(#member).get_to(parseOut->memberName); \
  }

#define JSON_MAP_TO_GETTER_SETTER(member, getter, setter, DataType) \
  if (isParsing) \
  { \
    parseOut->setter((*parseIn).at(#member).get<DataType>()); \
  } \
  else \
  { \
    (*exportOut)[#member] = exportIn->getter(); \
  }

#define JSON_MAP_TO_SETTER(member, setter, DataType) \
  if (isParsing) \
  { \
    parseOut->setter((*parseIn).at(#member).get<DataType>()); \
  }

#define JSON_MAP_TO_SETTER_OPTIONAL(member, setter, DataType) \
  if (isParsing && (*parseIn).contains(#member)) \
  { \
    parseOut->setter((*parseIn).at(#member).get<DataType>()); \
  }

#define JSON_MAP_TO_FACTORY_SETTER(member, setter, DataType) \
  if (isParsing) \
  { \
    auto newPtr = ZEngine::Factory::CreateInstance<DataType>(#DataType, DataType::GetStaticType()); \
    from_json(parseIn->at(#member).get<nlohmann::json>(), *newPtr); \
  }

#define JSON_MAP_TO_FACTORY_SETTER_OPTIONAL(member, setter, DataType) \
  if (isParsing && (*parseIn).contains(#member)) \
  { \
    auto newPtr = ZEngine::Factory::CreateInstance<DataType>(#DataType, DataType::GetStaticType()); \
    from_json(parseIn->at(#member).get<nlohmann::json>(), *newPtr); \
  }

#define JSON_ON_SERIALIZATION(op) \
  if (!isParsing) \
  { \
    op(*exportOut, *exportIn); \
  }

#define CUSTOM_JSON_DESERIALIZATION(op) \
  if (isParsing) \
  { \
    op(*parseIn, *parseOut); \
  }

#define INCLUDE_ASSET_REFERENCES() \
  auto assetManager = ZEngine::AssetManager::GetInstance(); \

#define _INTERNAL_JSON_MAP_TO_ASSET_REF_SETTER(member, setter, DataType) \
  auto assetId = uuids::uuid::from_string((*parseIn).at(#member).get<std::string>()).value(); \
  std::string path; \
  ZEngine::ObjectType type; \
  if (assetManager->GetCatalog()->GetAssetPathFromID(assetId, path, type)) \
  { \
    auto asset = assetManager->FindAssetFromPath(path); \
    if (asset == nullptr) \
      asset = assetManager->LoadAsset(path, type); \
    if (asset != nullptr) \
      parseOut->setter(asset->Cast<DataType>()); \
  } \

#define JSON_MAP_TO_ASSET_REF_SETTER(member, setter, DataType) \
  if (isParsing) \
  { \
    _INTERNAL_JSON_MAP_TO_ASSET_REF_SETTER(member, setter, DataType) \
  }

#define JSON_MAP_TO_ASSET_REF_SETTER_OPTIONAL(member, setter, DataType) \
  if (isParsing && (*parseIn).contains(#member)) \
  { \
    _INTERNAL_JSON_MAP_TO_ASSET_REF_SETTER(member, setter, DataType) \
  }

#define _INTERNAL_JSON_MAP_TO_ASSET_REFS_SETTER(member, setter, DataType) \
  auto assetIds = (*parseIn).at(#member).get<std::vector<std::string>>(); \
  std::vector<DataType*> assets; \
  for (auto assetId : assetIds) \
  { \
    auto assetUuid = uuids::uuid::from_string(assetId).value(); \
    std::string path; \
    ZEngine::ObjectType type; \
    if (assetManager->GetCatalog()->GetAssetPathFromID(assetUuid, path, type)) \
    { \
      auto asset = assetManager->FindAssetFromPath(path); \
      if (asset == nullptr) \
        asset = assetManager->LoadAsset(path, type); \
      if (asset != nullptr) \
        assets.push_back(asset->Cast<DataType>()); \
    } \
  } \
  parseOut->setter(assets);

#define JSON_MAP_TO_ASSET_REFS_SETTER(member, setter, DataType) \
  if (isParsing) \
  { \
    _INTERNAL_JSON_MAP_TO_ASSET_REFS_SETTER(member, setter, DataType) \
  }

#define JSON_MAP_TO_ASSET_REFS_SETTER_OPTIONAL(member, setter, DataType) \
  if (isParsing && (*parseIn).contains(#member)) \
  { \
    _INTERNAL_JSON_MAP_TO_ASSET_REFS_SETTER(member, setter, DataType) \
  }

/**
 * Allow for serialization for the uuids::uuid class. 
 */
NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<uuids::uuid> {
  static void to_json(json& j, const uuids::uuid& opt) {
    j = uuids::to_string(opt);
  }

  static void from_json(const json& j, uuids::uuid& opt) {
    opt = uuids::uuid::from_string(j.get<std::string>()).value();
  }
};

template <>
struct adl_serializer<glm::vec2> {
  static void to_json(json& j, const glm::vec2& opt) {
    j = json{ opt.x, opt.y };
  }

  static void from_json(const json& j, glm::vec2& opt) {
    opt.x = j.at(0).get<float>();
    opt.y = j.at(1).get<float>();
  }
};

template <>
struct adl_serializer<glm::vec3> {
  static void to_json(json& j, const glm::vec3& opt) {
    j = json{ opt.x, opt.y, opt.z };
  }

  static void from_json(const json& j, glm::vec3& opt) {
    opt.x = j.at(0).get<float>();
    opt.y = j.at(1).get<float>();
    opt.z = j.at(2).get<float>();
  }
};

template <>
struct adl_serializer<glm::vec4> {
  static void to_json(json& j, const glm::vec4& opt) {
    j = json{ opt.x, opt.y, opt.z, opt.w };
  }

  static void from_json(const json& j, glm::vec4& opt) {
    opt.x = j.at(0).get<float>();
    opt.y = j.at(1).get<float>();
    opt.z = j.at(2).get<float>();
    opt.w = j.at(3).get<float>();
  }
};

template <>
struct adl_serializer<glm::quat> {
  static void to_json(json& j, const glm::quat& opt) {
    j = json{ opt.x, opt.y, opt.z, opt.w };
  }

  static void from_json(const json& j, glm::quat& opt) {
    opt.x = j.at(0).get<float>();
    opt.y = j.at(1).get<float>();
    opt.z = j.at(2).get<float>();
    opt.w = j.at(3).get<float>();
  }
};

template<>
struct adl_serializer<glm::mat3>
{
  static void to_json(json& j, const glm::mat3& opt)
  {
    j = json {
      opt[0][0], opt[0][1], opt[0][2],
      opt[1][0], opt[1][1], opt[1][2],
      opt[2][0], opt[2][1], opt[2][2]
    };
  }

  static void from_json(const json& j, glm::mat3& opt)
  {
    for (int i = 0; i < 9; i++)
    {
      opt[i / 3][i % 3] = j.at(i).get<float>();
    }
  }
};

template<>
struct adl_serializer<glm::mat4>
{
  static void to_json(json& j, const glm::mat4& opt)
  {
    j = json {
      opt[0][0], opt[0][1], opt[0][2], opt[0][3],
      opt[1][0], opt[1][1], opt[1][2], opt[1][3],
      opt[2][0], opt[2][1], opt[2][2], opt[2][3],
      opt[3][0], opt[3][1], opt[3][2], opt[3][3]
    };
  }

  static void from_json(const json& j, glm::mat4& opt)
  {
    for (int i = 0; i < 16; i++)
    {
      opt[i / 4][i % 4] = j.at(i).get<float>();
    }
  }
};
NLOHMANN_JSON_NAMESPACE_END
