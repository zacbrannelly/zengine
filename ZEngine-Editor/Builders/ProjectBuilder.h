#include <string>

class Project;
class Directory;

class ProjectBuilder
{
public:
  static Project* CreateProject(std::string name, std::string projectDirectory);

private:
  static std::string GenerateProjectFile(const Directory& projectDirectory, std::string name);
  static std::string GenerateProjectCSharpProjectFile(const Directory& projectDirectory, std::string name);
};
