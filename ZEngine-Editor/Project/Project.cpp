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
using namespace ZEngine;

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
  Directory workingDir(_projectFile->GetDirectory());
  _catalog.LoadCatalogFromProjectJson(projectJson, workingDir.GetAbsolutePath());
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

Directory Project::GetProjectDirectory() const
{
  return Directory(_projectFile->GetDirectory());
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

  // Inject the asset catalog into the project JSON.
  _catalog.SaveCatalogToProjectJson(projectJson, _projectFile->GetDirectory());

  _projectFile->WriteJson(projectJson);
}

AssetCatalog& Project::GetCatalog()
{
  return _catalog;
}

Project::~Project()
{
  delete _projectFile;
}
