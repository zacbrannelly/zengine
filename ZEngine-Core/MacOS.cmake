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
set(PHYSX_BIN_PATH "${CMAKE_SOURCE_DIR}/3rdparty/PhysX/physx/bin/mac.x86_64/checked")

target_link_libraries(ZEngine-Core
  PRIVATE glfw
  PRIVATE ${DOTNET_CORECLR}
  PRIVATE ${DOTNET_HOST}
)

################################################################################
# P/Invoke Shared Library
################################################################################

# Where the P/Invoke shared library will be located at runtime.
set(ZENGINE_INTEROP_DLL_IMPORT_PATH lib/libZEngine-PInvoke.dylib)

################################################################################
# C# Interop Project
################################################################################

add_custom_target(ZEngine-Interop-Dotnet ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_PATH} ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
  COMMAND ${CMAKE_COMMAND} -E env --unset=TARGETNAME ${DOTNET_EXECUTABLE} publish ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/ZEngine.Interop.csproj
  DEPENDS ZEngine-PInvoke
)

# Copy the interop dll to the output directory (where the binaries expect them to be at runtime).
add_custom_command(TARGET ZEngine-Interop-Dotnet POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/bin/Debug/net6.0/publish ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.Interop
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/ZEngine-Core.runtimeconfig.json ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Core.runtimeconfig.json
  COMMENT "Copy the ZEngine-Interop.dll (.NET library) to the output directory"
  DEPENDS ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
)

################################################################################
# C# Plugin Manager Project
################################################################################

set(ZENGINE_PLUGIN_MANAGER_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/Lib/ZEngine.PluginManager)
set(ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/projects/ZEngine.PluginManager)

# Copy the project to the output directory
add_custom_command(TARGET ZEngine-Core POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_PATH} ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
)

# Build & publish the project using custom target
add_custom_target(ZEngine-PluginManager-Dotnet ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_PATH} ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
  COMMAND ${CMAKE_COMMAND} -E env --unset=TARGETNAME ${DOTNET_EXECUTABLE} publish ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}/ZEngine.PluginManager.csproj
  DEPENDS ZEngine-Core
)

# Copy the plugin manager dll to the output directory.
add_custom_command(TARGET ZEngine-PluginManager-Dotnet POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}/bin/Debug/net6.0/publish ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.PluginManager
  COMMENT "Copy the ZEngine.PluginManager.dll to the output directory"
  DEPENDS ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
)
