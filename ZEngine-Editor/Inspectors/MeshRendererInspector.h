#pragma once

#include "Inspector.h"

namespace ZEngine
{
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
}
