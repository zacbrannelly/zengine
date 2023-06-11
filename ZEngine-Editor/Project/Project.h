#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <ZEngine-Core/Assets/AssetCatalog.h>


class Project {
public:
  Project();
  ~Project();

  void Load(std::string path);
  void Save();

  void SetName(std::string name);
  std::string GetName() const;

  AssetCatalog& GetCatalog();
private:
  CatalogEntry DeserializeCatalogEntry(const nlohmann::json& entry);
  nlohmann::json SerializeCatalogEntry(const CatalogEntry& entry);

  std::string _name;
  std::string _path;
  AssetCatalog _catalog;
};
