#include <ZEngine-Core/Misc/Singleton.h>
#include <ZEngine-Core/Map/Objects/ObjectType.h>
#include <map>

class Editor;
class BaseGizmoRenderer;

class GizmoSystem : public Singleton<GizmoSystem>
{
public:
  void Init(Editor* editor);
  void Shutdown();

  void RenderGizmos(int viewId);

private:
  Editor* _editor;
  std::map<ObjectType, BaseGizmoRenderer*> _objectTypeToRenderer;
};
