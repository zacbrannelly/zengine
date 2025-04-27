#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

namespace ZEngine
{
	class DirectionalLightInspector : public Inspector
	{
	public:
		DirectionalLightInspector();
		~DirectionalLightInspector();

		void RenderElement() override;
	};
}
