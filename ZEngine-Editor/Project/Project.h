#include <string>
#include <vector>
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

  void Build();
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
};
