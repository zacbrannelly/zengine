#pragma once
#include <ZEngine-Core/ImmediateUI/GUIWindow.h>

namespace ZEngine
{
	class Editor;
	class Entity;

	class SceneGraphWindow : public GUIWindow
	{
	public:
		SceneGraphWindow(Editor* editor);
		~SceneGraphWindow();

		void ProcessInput() override;
		void RenderInWindow() override;

	private:
		void RenderNode(Entity* node);

		Editor* _context;
	};
}
