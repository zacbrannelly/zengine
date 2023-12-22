#include "MaterialFactory.h"
#include "Material.h"
#include "StandardShaders.h"

Material* MaterialFactory::CreateUnlitVertexColorMaterial(std::string name)
{
  // Create the material using the unlit color shader.
  auto material = new Material(name);
  material->SetShader(StandardShaders::GetUnlitColorShader());

  return material;
}
