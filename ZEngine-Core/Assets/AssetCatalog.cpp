#include "AssetCatalog.h"
#include "Asset.h"
#include "../Utilities/Directory.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

using namespace std;
using namespace ZEngine;

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
				auto name = ResolveAssetNameFromPath(path);

				_catalog.push_back({ id, path, type, name });
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
	File jsonFile(jsonFilePath);
	try
	{
		auto json = jsonFile.ReadJson();
		return LoadCatalogFromProjectJson(json);
	}
	catch (std::exception)
	{
		std::cerr << "Failed to load project file: " << jsonFilePath << std::endl;
		return false;
	}
}

bool AssetCatalog::LoadCatalogFromProjectJson(const nlohmann::json& json, std::string bathPath)
{
	if (!json.contains("catalog"))
		return false;

	auto catalog = json["catalog"];
	for (auto& catalogItem : catalog)
	{
		auto id = uuids::uuid::from_string(catalogItem["id"].get<string>()).value();
		auto type = StringToObjectType(catalogItem["type"].get<string>());
		auto path = catalogItem["path"].get<string>();

		if (!bathPath.empty())
		{
			path = bathPath + "/" + path;
		}
		
		// TODO: Remove this conditional once all project files contain the name field.
		auto name = catalogItem.contains("name") 
			? catalogItem["name"].get<string>()
			: ResolveAssetNameFromPath(path);

		_catalog.push_back({ id, path, type, name });
	}

	return true;
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

bool AssetCatalog::SaveCatalogToProjectJson(nlohmann::json& json, std::string basePath)
{
	auto& catalog = json["catalog"] = nlohmann::json::array();
	for (const auto& entry : _catalog)
	{
		nlohmann::json entryJson;
		entryJson["id"] = uuids::to_string(entry.id);
		entryJson["name"] = entry.name;
		entryJson["type"] = ObjectTypeToString(entry.type);

		if (basePath.empty())
		{
			entryJson["path"] = entry.path;
		}
		else
		{
			// Convert the path back to a relative path.
			Directory workingDir(basePath);
			File entryFile(entry.path);
			entryJson["path"] = entryFile.GetRelativePath(workingDir.GetAbsolutePath());
		}

		catalog.push_back(entryJson);
	}
}

void AssetCatalog::RegisterAsset(Asset* asset)
{
	if (asset != nullptr)
		RegisterAsset(asset->GetPath(), asset->GetType());
}

void AssetCatalog::RegisterAsset(std::string path, ObjectType type)
{
	// TODO: Clean this up - move to a utility class or something.
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);
	uuids::uuid_random_generator gen{generator};

	auto assetName = ResolveAssetNameFromPath(path);
	_catalog.push_back({ gen(), path, type, assetName });
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

std::string AssetCatalog::ResolveAssetNameFromPath(const std::string& path)
{
	File assetFile(path);
	auto extension = assetFile.GetExtension();
	if (extension == "asset" || extension == "shader")
	{
		auto json = assetFile.ReadJson();
		if (json.contains("name"))
			return json["name"].get<string>();
		else if (json.contains("class"))
			return json["class"].get<string>();
	}

	return assetFile.GetFilename();
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
