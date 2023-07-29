#include <string>
#include <vector>
#include <future>
#include <nlohmann/json.hpp>
#include <ZEngine-Core/Assets/AssetCatalog.h>

#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>

class Project {
public:
  Project();
  ~Project();

  void Load(std::string projectFilePath);
  void Save();

  bool Build(bool loadIntoScriptSystem = false);
  std::future<bool> BuildAsync(bool loadIntoScriptSystem = false);
  std::future<bool> BuildAndLoadAsync();
  bool IsBuildInProgress() const;

  std::string GetAssemblyPath() const;
  std::string GetAssemblyName() const;
  std::string GetScriptProjectPath() const;

  void SetName(std::string name);
  std::string GetName() const;

  AssetCatalog& GetCatalog();
private:
  CatalogEntry DeserializeCatalogEntry(const nlohmann::json& entry);
  nlohmann::json SerializeCatalogEntry(const CatalogEntry& entry);

  std::string _name;
  File* _projectFile;
  AssetCatalog _catalog;
  bool _buildInProgress;
};
