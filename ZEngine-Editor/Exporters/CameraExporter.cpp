#include "CameraExporter.h"
#include <ZEngine-Core/Component/Camera.h>

using namespace nlohmann;

json CameraExporter::ExportImpl(ZObject* obj)
{
	auto camera = static_cast<Camera*>(obj);

	json result;

	result["name"] = camera->GetName();
	result["type"] = camera->GetType();
	result["clearColor"] = GetVec4(camera->GetClearColor());
	result["clearFlags"] = camera->GetClearFlags();
	result["viewport"] = 
	{ 
		camera->GetViewportX(), 
		camera->GetViewportY(), 
		camera->GetViewportWidth(),
		camera->GetViewportHeight() 
	};

	result["projection"] = (uint32_t)camera->GetProjectionMode();
	result["fieldOfView"] = glm::degrees(camera->GetFieldOfView());
	result["zNear"] = camera->GetNear();
	result["zFar"] = camera->GetFar();
	result["orthoSize"] = camera->GetOrthoSize();
	result["renderToTexture"] = camera->IsRenderingToTexture();
	result["viewId"] = camera->GetViewId();

	return result;
}