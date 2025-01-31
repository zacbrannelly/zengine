#include <ZEngine-Core/ImmediateUI/Container.h>

namespace ZEngine
{
  class Editor;
  class MapController;

  class EditorToolbar : public Container
  {
  public:
    EditorToolbar(Editor* editor);
    ~EditorToolbar();

    void RenderElement() override;

  private:
    void RenderPlayButton();
    void RenderPauseAndStopButtons();

    Editor* _editor;
    MapController* _mapController;
  };
}
