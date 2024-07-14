#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>
#include <vector>

class Editor;
class GUITextField;
struct ComponentInfo;

class AddComponentPopup : public GUIDialog
{
public:
	AddComponentPopup(Editor* editor);
	~AddComponentPopup();

  void RegisterComponent(ComponentInfo component);

	void ProcessInput() override;
	void RenderInWindow() override;

private:
  Editor* _editor;
	GUITextField* _searchField;
  std::vector<ComponentInfo> _components;
};
