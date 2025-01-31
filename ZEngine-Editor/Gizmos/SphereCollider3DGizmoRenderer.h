#include "BaseGizmoRenderer.h"

namespace ZEngine
{
  class Mesh;
  class Material;

  class SphereCollider3DGizmoRenderer : public BaseGizmoRenderer
  {
  public:
    SphereCollider3DGizmoRenderer();
    ~SphereCollider3DGizmoRenderer();

    void Render(int viewId, const std::vector<Component*>& components) override;

  private:
    Mesh* _outlineMesh;
    Material* _outlineMaterial;
  };
}
