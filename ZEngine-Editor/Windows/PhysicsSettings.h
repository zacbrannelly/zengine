#include "../UI/GUIWindow.h"

class Editor;

class PhysicsSettings : public GUIWindow
{
public:
  PhysicsSettings(Editor* editor);
  ~PhysicsSettings();

  void ProcessInput() override;
	void RenderInWindow() override;

private:
  Editor* _editor;
};
