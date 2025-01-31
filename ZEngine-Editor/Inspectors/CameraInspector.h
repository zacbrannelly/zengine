#pragma once
#include "Inspector.h"

namespace ZEngine
{
	class CameraInspector : public Inspector
	{
	public:
		CameraInspector();
		~CameraInspector();

		void RenderElement() override;
	};
}
