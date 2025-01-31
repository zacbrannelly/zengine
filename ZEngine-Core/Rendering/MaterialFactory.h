#include <string>
#include <glm/glm.hpp>

namespace ZEngine
{
  class Material;

  class MaterialFactory
  {
  public:
    static Material* CreateUnlitVertexColorMaterial(std::string name);
  };
}
