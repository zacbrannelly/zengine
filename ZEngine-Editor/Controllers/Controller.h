#include "../UI/GUIElement.h"

class Controller : public GUIElement
{
public:
  virtual void Update() override = 0;

  virtual GUIElementType GetType() override
  {
    return GUI_TYPE_CONTROLLER;
  }

  virtual void RenderElement() override
  {
    // Controllers don't typically render
  }
};
