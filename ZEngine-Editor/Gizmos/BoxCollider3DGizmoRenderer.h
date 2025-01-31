#include "BaseGizmoRenderer.h"

namespace ZEngine
{
  class Mesh;
  class Material;

  class BoxCollider3DGizmoRenderer : public BaseGizmoRenderer
  {
  public:
    BoxCollider3DGizmoRenderer();
    ~BoxCollider3DGizmoRenderer();

    void Render(int viewId, const std::vector<Component*>& components) override;

  private:
    Mesh* _cubeOutlineMesh;
    Material* _cubeOutlineMaterial;
  };
}
