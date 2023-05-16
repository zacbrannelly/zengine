#pragma once

#include "ComponentImporter.h"

class AudioSourceImporter : public ComponentImporter<AudioSourceImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;

};

