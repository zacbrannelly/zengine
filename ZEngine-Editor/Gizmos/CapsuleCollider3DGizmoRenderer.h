#include "BaseGizmoRenderer.h"

namespace ZEngine
{
  class Mesh;
  class Material;

  class CapsuleCollider3DGizmoRenderer : public BaseGizmoRenderer
  {
  public:
    CapsuleCollider3DGizmoRenderer();
    ~CapsuleCollider3DGizmoRenderer();

    void Render(int viewId, const std::vector<Component*>& components) override;

  private:
    Mesh* _hemiSphereOutlineMesh;
    Mesh* _cylinderOutlineMesh;
    Material* _outlineMaterial;
  };
}
