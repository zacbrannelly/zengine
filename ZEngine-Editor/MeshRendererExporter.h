#pragma once

#include "ComponentExporter.h"

class MaterialAsset;
class Material;
class ModelAsset;
class Mesh;

class MeshRendererExporter : public ComponentExporter
{
public:
	nlohmann::json ExportImpl(ZObject* obj) override;

private:
	MaterialAsset* GetMaterialAsset(Material* mat) const;
	ModelAsset* GetModelAsset(Mesh* mesh) const;
};