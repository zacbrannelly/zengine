#pragma once
#include "Inspector.h"
class CameraInspector : public Inspector
{
public:
	CameraInspector();
	~CameraInspector();

	void RenderElement() override;
};

