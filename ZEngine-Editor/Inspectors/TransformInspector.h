#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

class TransformInspector : public Inspector
{
public:
	TransformInspector();
	~TransformInspector();

	void RenderElement() override;
private:
	glm::vec3 _rotationEulerDegrees;
	Component* _lastComponentInspected;
	bool _initialized;
};

