#include <string>
#include <ZEngine-Core/Map/Objects/ObjectType.h>

namespace ZEngine
{
  typedef struct ComponentInfo
  {
    std::string name;
    std::string description;
    ObjectType type;
  } ComponentInfo;
}
