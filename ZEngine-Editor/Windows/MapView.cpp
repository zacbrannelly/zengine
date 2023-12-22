#include "MapView.h"
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Transform.h>
#include <glm/gtc/type_ptr.hpp>

#include "MapViewToolbar.h"
#include "../Editor.h"
#include "../Inspectors/TransformInspector.h"
#include "../Inspectors/CameraInspector.h"
#include "../imgui-includes.h"


MapView::MapView(Editor* editor) : GUIWindow("Map View", 1024, 850, false)
{
	_editor = editor;

	// We must create an entity (so we can transform the camera) and translate it back 10 units
	_viewEntity = Factory::CreateInstance<Entity>("View Object", ObjectType::ENTITY);
	_viewEntity->GetTransform()->SetPosition(glm::vec3(0, 0, -10));

	// Create camera on view 1, this will be used to view/render the scene
	_viewCamera = Factory::CreateInstance<Camera>("Camera", ObjectType::CAMERA);	
	_viewEntity->AddComponent(_viewCamera, false);

	// Set default values for the camera
	_viewCamera->SetProjectionMode(Camera::ProjectionMode::ORTHOGRAPHIC);
	_viewCamera->SetFieldOfView(60.0f);
	_viewCamera->SetViewport(0, 0, 1920, 1080);
	_viewCamera->SetClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	_viewCamera->SetViewId(1);
	_viewCamera->SetRenderToTexture(true);

	// Make image linked to the view camera
	_viewImage = new GUIImage(_viewCamera->GetRenderTexture(), _viewCamera->GetViewportWidth(), _viewCamera->GetViewportHeight());
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	_viewImage->FlipVertically();
#endif
    
    _mapViewToolbar = new MapViewToolbar(_editor, this);

	// Add elements to the window
	Add(_mapViewToolbar);
	Add(_viewImage);

	_aspectRatioW = 16;
	_aspectRatioH = 9;

	_transformInspector = new TransformInspector();
	_cameraInspector = new CameraInspector();

	_transformInspector->Inspect(_viewEntity->GetTransform());
	_cameraInspector->Inspect(_viewCamera);

	// The window won't be resizable, it will fit to content
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void MapView::ProcessInput()
{
}

void MapView::RenderInWindow()
{
	_viewImage->SetSize(GetContentWidth(), GetContentWidth() * (_aspectRatioH / _aspectRatioW));
    
    // Render where the view image is positioned in screen space.
    auto viewImagePos = _viewImage->GetScreenPosition();
    ImGuizmo::SetRect(viewImagePos.x, viewImagePos.y, _viewImage->GetWidth(), _viewImage->GetHeight());
    
    // Make sure the guizmo is rendered using the current window's draw list.
    ImGuizmo::SetDrawlist();

    // Render Translation/Rotate/Scale Gizmos for the selected object.
    auto selectedEntity = _editor->GetSelectedEntity();
    if (selectedEntity != nullptr)
    {
        const float* projMatrix = glm::value_ptr(_viewCamera->GetProjectionMatrix());
        const float* viewMatrix = glm::value_ptr(_viewCamera->GetViewMatrix());
        
        float modelMatrix[16];
        memcpy(modelMatrix, glm::value_ptr(selectedEntity->GetTransform()->GetWorldTransformMatrix()), sizeof(float) * 16);

        if (ImGuizmo::Manipulate(viewMatrix, projMatrix, _mapViewToolbar->GetObjectGizmoOp(), _mapViewToolbar->GetObjectGizmoMode(), modelMatrix))
        {
            float translation[3];
            float rotation[3];
            float scale[3];
            ImGuizmo::DecomposeMatrixToComponents(modelMatrix, translation, rotation, scale);
            
            selectedEntity->GetTransform()->SetPosition({ translation[0], translation[1], translation[2] });
            selectedEntity->GetTransform()->SetRotation({ rotation[0], rotation[1], rotation[2] });
            selectedEntity->GetTransform()->SetScale({ scale[0], scale[1], scale[2] });
        }
    }

	// Allow the user to change the size of the texture being rendered in the window
	// TODO: Move this to its own component.
	if (ImGui::CollapsingHeader("View Settings"))
	{
		float aspectRatio[] = { _aspectRatioW, _aspectRatioH };
		if (ImGui::InputFloat2("Aspect Ratio", aspectRatio, "%.1f"))
		{
			_aspectRatioW = aspectRatio[0];
			_aspectRatioH = aspectRatio[1];

			_viewImage->SetSize(GetContentWidth(), GetContentWidth() * (_aspectRatioH / _aspectRatioW));
		}
	}

	// Draw the inspectors for the camera transform and camera settings
	// TODO: Move this to its own component.
	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		_transformInspector->RenderElement();
		ImGui::Spacing();
		_cameraInspector->RenderElement();
	}
}

void MapView::Update()
{
}

void MapView::RenderElement()
{
	// Ensure the render texture is current, as it changes when viewport is changed
	_viewImage->SetTexture(_viewCamera->GetRenderTexture());

	// Set camera settings
	_viewCamera->Render(-1);

	// Render the world without the internal cameras
	if (_editor->GetSelectedMap() != nullptr)
	{
		auto map = _editor->GetSelectedMap();
		auto viewId = _viewCamera->GetViewId();
		map->RenderWorld(viewId);
		map->RenderGizmos(viewId);
	}

	// Render the actual texture to the screen (more like submit the draw call to bgfx)
	GUIWindow::RenderElement();
}

MapView::~MapView()
{
}
