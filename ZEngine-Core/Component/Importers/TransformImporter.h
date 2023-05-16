#pragma once

#include "ComponentImporter.h"

class TransformImporter : public ComponentImporter<TransformImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};

