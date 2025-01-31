#include <ZEngine-Core/ImmediateUI/GUIWindow.h>

namespace ZEngine
{
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
}
