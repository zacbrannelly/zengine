#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>
#include <ZEngine-Core/ImmediateUI/GUIImage.h>
#include <ZEngine-Core/Rendering/Graphics.h>

namespace ZEngine
{
	class Editor;
	class GizmoSystem;
	class Camera;
	class Entity;
	class Map;
	class MapViewToolbar;
	class MapViewPicker;

	class MapView : public GUIWindow
	{
	public:
		MapView(Editor* map);
		~MapView();

		void ProcessInput() override;
		void RenderInWindow() override;
		void RenderElement() override;

		Camera* GetCamera() const;

	private:
		GUIImage* _viewImage;
		Entity* _viewEntity;
		Camera* _viewCamera;
		Editor* _editor;
		GizmoSystem* _gizmoSystem;
		MapViewToolbar* _mapViewToolbar;
		MapViewPicker* _mapViewPicker;
		bool _cameraInFlight;
	};
}
