#pragma once

#include "Inspector.h"

class TransformInspector : public Inspector
{
public:
	TransformInspector();
	~TransformInspector();

	void RenderElement() override;
};

