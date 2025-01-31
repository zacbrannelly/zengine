#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

namespace ZEngine
{
	class SphereCollider3DInspector : public Inspector
	{
	public:
		SphereCollider3DInspector();
		~SphereCollider3DInspector();

		void RenderElement() override;
	};
}
