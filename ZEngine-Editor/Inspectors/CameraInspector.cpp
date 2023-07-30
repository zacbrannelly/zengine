#include "CameraInspector.h"
#include "../imgui-includes.h"
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Component/Transform.h>


CameraInspector::CameraInspector()
{
}

void CameraInspector::RenderElement()
{
	if (GetValue() == nullptr) return;

	auto camera = static_cast<Camera*>(GetValue());

	ImGui::BeginChild("Map View Camera Settings", ImVec2(0, 288));
	{
		std::string clearString = "";
		switch (camera->GetClearFlags())
		{
		case BGFX_CLEAR_COLOR:
			clearString = "Just Color";
			break;
		case BGFX_CLEAR_DEPTH:
			clearString = "Just Depth";
			break;
		case BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH:
			clearString = "Color & Depth";
			break;
		default:
			clearString = "Don't Clear";
		}

		if (ImGui::BeginCombo("Clear Flags", clearString.c_str()))
		{
			if (ImGui::Selectable("Color & Depth"))
			{
				camera->SetClearFlags(BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
			}

			if (ImGui::Selectable("Just Color"))
			{
				camera->SetClearFlags(BGFX_CLEAR_COLOR);
			}

			if (ImGui::Selectable("Just Depth"))
			{
				camera->SetClearFlags(BGFX_CLEAR_DEPTH);
			}

			if (ImGui::Selectable("Don't Clear"))
			{
				camera->SetClearFlags(BGFX_CLEAR_NONE);
			}

			ImGui::EndCombo();
		}

		float clearColor[4] = { camera->GetClearColor().r, camera->GetClearColor().g, camera->GetClearColor().b, camera->GetClearColor().a };
		if (ImGui::ColorEdit4("Clear Color", clearColor))
		{
			camera->SetClearColor({ clearColor[0], clearColor[1], clearColor[2], clearColor[3] });
		}

		int viewport[4] = { camera->GetViewportX(), camera->GetViewportY(), camera->GetViewportWidth(), camera->GetViewportHeight() };
		if (ImGui::InputInt4("Viewport", viewport))
		{
			camera->SetViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		}

		if (ImGui::BeginCombo("Projection", camera->GetProjectionMode() == Camera::PERSPECTIVE ? "Perspective" : "Orthograpic"))
		{
			if (ImGui::Selectable("Orthographic"))
			{
				camera->SetProjectionMode(Camera::ORTHOGRAPHIC);
			}

			if (ImGui::Selectable("Perspective"))
			{
				camera->SetProjectionMode(Camera::PERSPECTIVE);
			}

			ImGui::EndCombo();
		}

		if (camera->GetProjectionMode() == Camera::PERSPECTIVE)
		{
			float fov = camera->GetFieldOfView();

			if (ImGui::SliderAngle("Field of View", &fov))
			{
				camera->SetFieldOfView(glm::degrees(fov));
			}
		}
		else
		{
			float size = camera->GetOrthoSize();

			if (ImGui::SliderFloat("Size", &size, 0.0f, 50.0f))
			{
				camera->SetOrthoSize(size);
			}
		}

		float near = camera->GetNear();
		if (ImGui::DragFloat("Near", &near))
		{
			camera->SetNear(near);
		}

		float far = camera->GetFar();
		if (ImGui::DragFloat("Far", &far))
		{
			camera->SetFar(far);
		}
	}
	ImGui::EndChild();
}

CameraInspector::~CameraInspector()
{
}
