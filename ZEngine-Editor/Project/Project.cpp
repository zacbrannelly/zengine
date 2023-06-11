#include "Project.h"
#include <fstream>
#include <stdexcept>
#include <uuid.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include <ZEngine-Core/Utilities/File.h>

using namespace nlohmann;

Project::Project() 
{
}

void Project::Load(std::string path)
{
  if (_path != "") {
    throw std::runtime_error("Project already loaded!");
  }

  _path = path;

  // Parse the project file (JSON)
  json projectJson;
  std::ifstream projectFile(path);
  projectFile >> projectJson;

  // Load the name of the project.
  _name = projectJson["name"].get<std::string>();

  // Load the asset catalog (mapping between UUIDs and paths to the assets)
  auto catalogJson = projectJson["catalog"].get<json::array_t>();
  for (const auto& entryJson : catalogJson) {
    auto entry = DeserializeCatalogEntry(entryJson);
    _catalog.PushEntry(entry);
  }
}

void Project::SetName(std::string name)
{
  _name = name;
}

std::string Project::GetName() const
{
  return _name;
}

void Project::Save()
{
  if (_path == "") {
    throw std::runtime_error("Cannot save project that has not been loaded!");
  }

  json projectJson;
  projectJson["name"] = _name;

  json catalogJson;
  for (const auto& entry : _catalog.GetCatalogList()) {
    catalogJson.push_back(SerializeCatalogEntry(entry));
  }
  projectJson["catalog"] = catalogJson;

  std::ofstream projectFile(_path);
  projectFile << projectJson.dump(2);
}

CatalogEntry Project::DeserializeCatalogEntry(const json& entryJson)
{
  CatalogEntry entry;
  entry.id = uuids::uuid::from_string(entryJson["id"].get<std::string>()).value();
  entry.type = StringToObjectType(entryJson["type"].get<std::string>());
  entry.path = entryJson["path"].get<std::string>();

  // Convert the path to an absolute path.
  Directory workingDir(Directory::GetBasePath(_path));
  entry.path = workingDir.GetAbsolutePath() + "/" + entry.path;

  return entry;
}

json Project::SerializeCatalogEntry(const CatalogEntry& entry)
{
  json entryJson;
  entryJson["id"] = uuids::to_string(entry.id);
  entryJson["type"] = ObjectTypeToString(entry.type);

  // Convert the path back to a relative path.
  Directory workingDir(Directory::GetBasePath(_path));
  File entryFile(entry.path);
  entryJson["path"] = entryFile.GetRelativePath(workingDir.GetAbsolutePath());

  return entryJson;
}

AssetCatalog& Project::GetCatalog()
{
  return _catalog;
}

Project::~Project()
{
}
