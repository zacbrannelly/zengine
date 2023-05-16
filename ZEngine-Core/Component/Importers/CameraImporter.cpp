#include "CameraImporter.h"
#include "../Camera.h"
#include "../../Misc/Factory.h"

using namespace std;
using namespace nlohmann;

ZObject* CameraImporter::ImportImpl(std::string name, json& values)
{
	Camera* camera = Factory::CreateInstance<Camera>(name, CAMERA);

	if (HasKey("clearColor", values))
	{
		camera->SetClearColor(ReadVector<4>("clearColor", values));
	}

	if (HasKey("clearFlags", values))
	{
		camera->SetClearFlags(ReadUnsignedInt("clearFlags", values));
	}

	if (HasKey("viewport", values))
	{
		auto data = ReadArray<int, 4>("viewport", values);
		camera->SetViewport(data[0], data[1], data[2], data[3]);
	}

	if (HasKey("projection", values))
	{
		camera->SetProjectionMode((Camera::ProjectionMode)ReadUnsignedInt("projection", values));
	}

	if (HasKey("fieldOfView", values))
	{
		camera->SetFieldOfView(ReadFloat("fieldOfView", values));
	}

	if (HasKey("zNear", values))
	{
		camera->SetNear(ReadFloat("zNear", values));
	}

	if (HasKey("zFar", values))
	{
		camera->SetFar(ReadFloat("zFar", values));
	}

	if (HasKey("orthoSize", values))
	{
		camera->SetOrthoSize(ReadFloat("orthoSize", values));
	}

	if (HasKey("renderToTexture", values))
	{
		camera->SetRenderToTexture(ReadBool("renderToTexture", values));
	}

	if (HasKey("viewId", values))
	{
		camera->SetViewId(ReadInt("viewId", values));
	}

	return camera;
}
