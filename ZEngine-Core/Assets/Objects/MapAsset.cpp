#include "MapAsset.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace nlohmann;

MapAsset::MapAsset(string name) : Asset(name, ObjectType::MAP_ASSET)
{
}

bool MapAsset::Load(string path)
{
	ifstream in(path, ios::in);

	if (!in.is_open())
	{
		cout << "MAP_ASSET: Failed to load file: " << path << endl;
		return false;
	}


	return true;
}

Map* MapAsset::GetMap() const
{
	return _map;
}

MapAsset::~MapAsset()
{
}
