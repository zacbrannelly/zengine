#include "MaterialAsset.h"
#include "../../json/json.hpp"


MaterialAsset::MaterialAsset(std::string name) : Asset(name, ObjectType::MATERIAL_ASSET)
{
}

bool MaterialAsset::Load(std::string path)
{
	return false;
}

MaterialAsset::~MaterialAsset()
{
}
