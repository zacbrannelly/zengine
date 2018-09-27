#pragma once

#include "../Asset.h"

class MaterialAsset : public Asset
{
public:
	MaterialAsset(std::string name);
	~MaterialAsset();

	bool Load(std::string path) override;
};

