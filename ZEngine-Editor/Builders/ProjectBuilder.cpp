#include <iostream>
#include <stdexcept>
#include <inja.hpp>
#include <nlohmann/json.hpp>
#include <ZEngine-Core/Utilities/Directory.h>

#include "ProjectBuilder.h"
#include "../Project/Project.h"

using namespace nlohmann;

Project* ProjectBuilder::CreateProject(std::string name, std::string projectDirectory)
{
  // Check if the project directory already exists
  Directory projectDir(projectDirectory + "/" + name);
  if (projectDir.Exists()) {
    throw std::runtime_error("Project directory already exists.");
  }

  // Create the project directory
  projectDir.Create();

  // Generate required project files.
  auto projectFilePath = GenerateProjectFile(projectDir, name);
  auto csprojFilePath = GenerateProjectCSharpProjectFile(projectDir, name);

  std::cout << "Created project at " << projectDir.GetAbsolutePath() << std::endl;
  std::cout << "Created project file at " << projectFilePath << std::endl;
  std::cout << "Created C# project file at " << csprojFilePath << std::endl;

  auto project = new Project();
  project->Load(projectFilePath);

  return project;
}

std::string ProjectBuilder::GenerateProjectFile(const Directory& projectDirectory, std::string name)
{
  // Load the .zproj file template
  inja::Environment injaEnv;
  inja::Template projectTemplate = injaEnv.parse_template("templates/project/BaseProject/BaseProject.zproj");

  // Generate the project file
  json data;
  data["projectName"] = name;

  auto projectFilePath = projectDirectory.GetAbsolutePath() + "/" + name + ".zproj";
  injaEnv.write(projectTemplate, data, projectFilePath);

  return projectFilePath;
}

std::string ProjectBuilder::GenerateProjectCSharpProjectFile(const Directory& projectDirectory, std::string name)
{
  // Load the .csproj file template
  inja::Environment injaEnv;
  inja::Template projectTemplate = injaEnv.parse_template("templates/project/BaseProject/BaseProject.Scripts.csproj");

  // Generate the path to the ZEngine.Interop.dll file
  auto cwd = Directory::GetCurrentWorkingDirectory();
  auto interopDllPath = cwd + "/lib/ZEngine.Interop/ZEngine.Interop.dll";

  // Generate the project file
  json data;
  data["interopDllPath"] = interopDllPath;

  auto projectFilePath = projectDirectory.GetAbsolutePath() + "/" + name + ".Scripts.csproj";
  injaEnv.write(projectTemplate, data, projectFilePath);

  return projectFilePath;
}
