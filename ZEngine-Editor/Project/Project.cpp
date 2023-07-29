#include "Project.h"
#include <fstream>
#include <stdexcept>
#include <thread>
#include <cstdlib>
#include <uuid.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Scripting/CSharp/CSharpScriptSystem.h>

using namespace nlohmann;

Project::Project() : _projectFile(nullptr), _buildInProgress(false)
{
}

void Project::Load(std::string projectFilePath)
{
  if (_projectFile != nullptr) {
    throw std::runtime_error("Project already loaded!");
  }

  _projectFile = new File(projectFilePath);

  // Parse the JSON from the project file.
  json projectJson = _projectFile->ReadJson();

  // Load the name of the project.
  _name = projectJson["name"].get<std::string>();

  // Load the asset catalog (mapping between UUIDs and paths to the assets)
  auto catalogJson = projectJson["catalog"].get<json::array_t>();
  for (const auto& entryJson : catalogJson) {
    auto entry = DeserializeCatalogEntry(entryJson);
    _catalog.PushEntry(entry);
  }
}

std::future<bool> Project::BuildAsync(bool loadIntoScriptSystem)
{
  return std::async(std::launch::async, [this, loadIntoScriptSystem]() {
    return Build(loadIntoScriptSystem);
  });
}

std::future<bool> Project::BuildAndLoadAsync()
{
  return BuildAsync(true);
}

bool Project::Build(bool loadIntoScriptSystem)
{
  // Make sure the project has been loaded
  if (_projectFile == nullptr)
  {
    throw std::runtime_error("Cannot build project that has not been loaded!");
  }

  // TODO: Make sure there isn't already a build in progress

  // Locate the project file (e.g. /path/to/project/ProjectName.Scripts.csproj)
  File csharpProjectFile(GetScriptProjectPath());
  if (!csharpProjectFile.Exists())
  {
    throw std::runtime_error("Failed to locate C# project file!");
  }

  _buildInProgress = true;

  // Build the C# project and output the DLL to the bin directory of the project folder.
  auto scriptSystem = CSharpScriptSystem::GetInstance();
  auto dllOutputPath = GetAssemblyPath();
  auto success = scriptSystem->BuildProject(csharpProjectFile.GetPath(), dllOutputPath);

  // Optionally load the built assembly into the scripting system.
  if (loadIntoScriptSystem && success) {
    scriptSystem->LoadProjectAssembly(dllOutputPath); 
  }

  _buildInProgress = false;

  return success;
}

bool Project::IsBuildInProgress() const
{
  return _buildInProgress;
}

std::string Project::GetAssemblyPath() const
{
  // Make sure the project has been loaded
  if (_projectFile == nullptr)
  {
    throw std::runtime_error("Cannot get assembly path for project that has not been loaded!");
  }

  return _projectFile->GetDirectory() + "/bin/" + _name + ".Scripts.dll";
}

std::string Project::GetAssemblyName() const
{
  // Make sure the project has been loaded
  if (_projectFile == nullptr)
  {
    throw std::runtime_error("Cannot get assembly name for project that has not been loaded!");
  }

  return _name + ".Scripts";
}

std::string Project::GetScriptProjectPath() const
{
  // Make sure the project has been loaded
  if (_projectFile == nullptr)
  {
    throw std::runtime_error("Cannot get script project path for project that has not been loaded!");
  }

  return _projectFile->GetDirectory() + "/" + _name + ".Scripts.csproj";
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
  if (_projectFile == nullptr) {
    throw std::runtime_error("Cannot save project that has not been loaded!");
  }

  json projectJson;
  projectJson["name"] = _name;

  json catalogJson;
  for (const auto& entry : _catalog.GetCatalogList()) {
    catalogJson.push_back(SerializeCatalogEntry(entry));
  }
  projectJson["catalog"] = catalogJson;

  _projectFile->WriteJson(projectJson);
}

CatalogEntry Project::DeserializeCatalogEntry(const json& entryJson)
{
  CatalogEntry entry;
  entry.id = uuids::uuid::from_string(entryJson["id"].get<std::string>()).value();
  entry.type = StringToObjectType(entryJson["type"].get<std::string>());
  entry.path = entryJson["path"].get<std::string>();

  // Convert the path to an absolute path.
  Directory workingDir(_projectFile->GetDirectory());
  entry.path = workingDir.GetAbsolutePath() + "/" + entry.path;

  return entry;
}

json Project::SerializeCatalogEntry(const CatalogEntry& entry)
{
  json entryJson;
  entryJson["id"] = uuids::to_string(entry.id);
  entryJson["type"] = ObjectTypeToString(entry.type);

  // Convert the path back to a relative path.
  Directory workingDir(_projectFile->GetDirectory());
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
  delete _projectFile;
}
