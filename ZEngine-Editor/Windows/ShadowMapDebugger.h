#include <ZEngine-Core/ImmediateUI/GUIWindow.h>

namespace ZEngine
{
  class Editor;
  class LightingSystem;

  class ShadowMapDebugger : public GUIWindow
  {
  public:
    ShadowMapDebugger(Editor* editor);
    ~ShadowMapDebugger();

    void ProcessInput() override;
    void RenderInWindow() override;
  private:
    Editor* _editor;
    LightingSystem* _lightingSystem;
    bgfx::TextureHandle _depthTexture { bgfx::kInvalidHandle };
    int _selectedShadowMapIndex { 0 };
  };
}
