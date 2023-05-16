#pragma once

#include "Inspector.h"

class MeshRendererInspector : public Inspector
{
public:
	MeshRendererInspector();
	~MeshRendererInspector();

	void Inspect(Component* comp) override;
	void RenderElement() override;

private:
	int _neededMaterials;
};

