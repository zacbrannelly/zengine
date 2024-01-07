#include "AssetCatalog.h"
#include "Asset.h"

#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

using namespace std;

AssetCatalog::AssetCatalog()
{
}

void AssetCatalog::ClearCatalog()
{
	_catalog.clear();
}

bool AssetCatalog::LoadCatalog(string path)
{
	ifstream in(path, ios::in);

	if (in.is_open())
	{
		string line;
	
		while (getline(in, line))
		{
			auto commaPos = line.find(',');
			auto equalPos = line.find('='); 

			if (commaPos == string::npos || equalPos == string::npos) continue;

			// FORMAT: id,type=path
			try
			{
				auto id = uuids::uuid::from_string(line.substr(0, commaPos)).value();
				auto type = StringToObjectType(line.substr(commaPos + 1, equalPos - commaPos - 1));
				auto path = line.substr(equalPos + 1, line.length() - equalPos - 1);

				_catalog.push_back({ id, path, type });
			}
			catch (std::exception) {}
		}

		in.close();

		_lastCatalogPath = path;
		return true;
	}

	return false;
}

bool AssetCatalog::LoadCatalogFromProjectJson(string jsonFilePath)
{
	ifstream in(jsonFilePath, ios::in);

	if (in.is_open())
	{
		nlohmann::json json;
		in >> json;

		auto catalog = json["catalog"];
		for (auto& catalogItem : catalog)
		{
			auto id = uuids::uuid::from_string(catalogItem["id"].get<string>()).value();
			auto type = StringToObjectType(catalogItem["type"].get<string>());
			auto path = catalogItem["path"].get<string>();

			_catalog.push_back({ id, path, type });
		}

		in.close();

		return true;
	}

	return false;
}

bool AssetCatalog::SaveCatalog(string path)
{
	ofstream out(path, ios::out);

	if (out.is_open())
	{
		for (const auto& item : _catalog)
		{
			// FORMAT: id,type=path
			out << item.id << "," << item.type << "=" << item.path << endl;
		}

		out.close();
		return true;
	}

	return false;
}

void AssetCatalog::RegisterAsset(Asset* asset)
{
	if (asset != nullptr)
		RegisterAsset(asset->GetPath(), asset->GetType());
}

void AssetCatalog::RegisterAsset(std::string path, ObjectType type)
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);
	uuids::uuid_random_generator gen{generator};

	_catalog.push_back({ gen(), path, type });
}

void AssetCatalog::PushEntry(CatalogEntry entry)
{
	_catalog.push_back(entry);
}

bool AssetCatalog::HasAsset(std::string assetPath)
{
	return any_of(_catalog.begin(), _catalog.end(), 
	[&assetPath](const auto& item) 
	{ 
		return item.path == assetPath;
	});
}

bool AssetCatalog::GetAssetPathFromID(uuids::uuid id, std::string& path, ObjectType& type)
{
	auto it = find_if(_catalog.begin(), _catalog.end(), [&id](auto item) { return item.id == id; });

	if (it != _catalog.end())
	{
		path = (*it).path;
		type = (ObjectType)(*it).type;

		return true;
	}

	return false;
}

uuids::uuid AssetCatalog::GetAssetIDFromPath(std::string path)
{
	auto it = find_if(_catalog.begin(), _catalog.end(), [&path](auto item) { return item.path == path; });

	if (it != _catalog.end())
	{
		return it->id;
	}

	return uuids::uuid();
}

std::vector<CatalogEntry> AssetCatalog::GetAssetsByType(ObjectType type)
{
	std::vector<CatalogEntry> results;

	for (auto& entry : _catalog)
	{
		if (entry.type == type)
			results.push_back(entry);
	}

	return results;
}

const std::vector<CatalogEntry>& AssetCatalog::GetCatalogList() const
{
	return _catalog;
}

const std::string& AssetCatalog::GetLastCatalogPath() const
{
	return _lastCatalogPath;
}

AssetCatalog::~AssetCatalog()
{
}
