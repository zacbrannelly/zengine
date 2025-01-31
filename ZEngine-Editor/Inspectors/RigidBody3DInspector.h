#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

namespace ZEngine
{
	class RigidBody3DInspector : public Inspector
	{
	public:
		RigidBody3DInspector();
		~RigidBody3DInspector();

		void RenderElement() override;
	};
}
