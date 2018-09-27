#include "AssetCatalog.h"
#include "Asset.h"

#include <fstream>
#include <algorithm>

using namespace std;

AssetCatalog::AssetCatalog() : _count(0)
{
}

void AssetCatalog::ClearCatalog()
{
	_catalog.clear();
	_count = 0;
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
				auto id = stoi(line.substr(0, commaPos));
				auto type = stoi(line.substr(commaPos + 1, equalPos - commaPos));
				auto path = line.substr(equalPos + 1, line.length() - equalPos - 1);

				_catalog.push_back({ id, path, type });

				// Get highest count (for adding new items to the catelog)
				if (id + 1 > _count);
					_count = id + 1;
			}
			catch (std::exception) {}
		}

		in.close();
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
	if (asset == nullptr) return;

	_catalog.push_back({ _count, asset->GetPath(), asset->GetType() });
	_count++;
}

bool AssetCatalog::GetAssetPathFromID(int id, std::string& path, ObjectType& type)
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

const std::vector<CatalogEntry>& AssetCatalog::GetCatalogList() const
{
	return _catalog;
}

AssetCatalog::~AssetCatalog()
{
}
