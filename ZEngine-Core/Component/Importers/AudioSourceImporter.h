#pragma once

#include "ComponentImporter.h"

class AudioSourceImporter : public ComponentImporter<AudioSourceImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json::object_t& values) override;

};

