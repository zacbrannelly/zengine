#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

namespace ZEngine
{
	class BoxCollider3DInspector : public Inspector
	{
	public:
		BoxCollider3DInspector();
		~BoxCollider3DInspector();

		void RenderElement() override;
	};
}
