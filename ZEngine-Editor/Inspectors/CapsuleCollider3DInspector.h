#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

class CapsuleCollider3DInspector : public Inspector
{
public:
	CapsuleCollider3DInspector();
	~CapsuleCollider3DInspector();

	void RenderElement() override;
};
