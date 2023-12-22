#include <string>
#include <glm/glm.hpp>

class Material;

class MaterialFactory
{
public:
  static Material* CreateUnlitVertexColorMaterial(std::string name);
};
