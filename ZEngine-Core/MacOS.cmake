################################################################################
# Linking
################################################################################

set(DOTNET_LIBRARY_PATH "/usr/local/share/dotnet/shared/Microsoft.NETCore.App/6.0.14")
set(DOTNET_HOST_PATH "/usr/local/share/dotnet/packs/Microsoft.NETCore.App.Host.osx-arm64/6.0.14/runtimes/osx-arm64/native")

find_library(
  DOTNET_CORECLR
  NAMES coreclr libcoreclr
  HINTS "${DOTNET_LIBRARY_PATH}"
  NO_DEFAULT_PATH
)

find_library(
  DOTNET_HOST
  NAMES nethost libnethost
  HINTS "${DOTNET_HOST_PATH}"
  NO_DEFAULT_PATH
)

# Set RPATH (where to look for shared libraries, i.e. the dotnet runtime)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_RPATH "${DOTNET_LIBRARY_PATH};${DOTNET_HOST_PATH}")

# Point to the correct PhysX binaries to statically link against
set(PHYSX_BIN_PATH "${CMAKE_SOURCE_DIR}/3rdparty/PhysX/physx/bin/ios64/checked")

target_link_libraries(ZEngine-Core
  PRIVATE glfw
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE ${DOTNET_CORECLR}
  PRIVATE ${DOTNET_HOST}
)
