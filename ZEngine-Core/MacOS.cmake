################################################################################
# Linking
################################################################################

# Resolve the real dotnet installation directory from the executable CMake found.
get_filename_component(DOTNET_EXECUTABLE_REALPATH "${DOTNET_EXECUTABLE}" REALPATH)
get_filename_component(DOTNET_ROOT "${DOTNET_EXECUTABLE_REALPATH}" DIRECTORY)

# Map the current macOS CPU architecture to the .NET runtime identifier naming.
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
  set(DOTNET_RUNTIME_IDENTIFIER "osx-arm64")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set(DOTNET_RUNTIME_IDENTIFIER "osx-x64")
else()
  message(FATAL_ERROR "Unsupported macOS processor for .NET host resolution: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# These roots are stable; the version folder below them changes with each .NET install/update.
set(DOTNET_LIBRARY_ROOT "${DOTNET_ROOT}/shared/Microsoft.NETCore.App")
set(DOTNET_HOST_ROOT "${DOTNET_ROOT}/packs/Microsoft.NETCore.App.Host.${DOTNET_RUNTIME_IDENTIFIER}")

# Discover all installed runtime/host pack versions so we do not hard-code a specific SDK version.
file(GLOB DOTNET_LIBRARY_VERSIONS RELATIVE "${DOTNET_LIBRARY_ROOT}" "${DOTNET_LIBRARY_ROOT}/*")
file(GLOB DOTNET_HOST_VERSIONS RELATIVE "${DOTNET_HOST_ROOT}" "${DOTNET_HOST_ROOT}/*")

if(NOT DOTNET_LIBRARY_VERSIONS)
  message(FATAL_ERROR "No .NET runtime versions found under ${DOTNET_LIBRARY_ROOT}")
endif()

if(NOT DOTNET_HOST_VERSIONS)
  message(FATAL_ERROR "No .NET host pack versions found under ${DOTNET_HOST_ROOT}")
endif()

# Sort naturally so version strings like 10.0.3 sort after 9.0.x, then pick the newest installed version.
list(SORT DOTNET_LIBRARY_VERSIONS COMPARE NATURAL ORDER DESCENDING)
list(SORT DOTNET_HOST_VERSIONS COMPARE NATURAL ORDER DESCENDING)

list(GET DOTNET_LIBRARY_VERSIONS 0 DOTNET_LIBRARY_VERSION)
list(GET DOTNET_HOST_VERSIONS 0 DOTNET_HOST_VERSION)

# Build the final directories that `find_library` and the runtime loader should search.
set(DOTNET_LIBRARY_PATH "${DOTNET_LIBRARY_ROOT}/${DOTNET_LIBRARY_VERSION}")
set(DOTNET_HOST_PATH "${DOTNET_HOST_ROOT}/${DOTNET_HOST_VERSION}/runtimes/${DOTNET_RUNTIME_IDENTIFIER}/native")

message(STATUS "Using .NET runtime path: ${DOTNET_LIBRARY_PATH}")
message(STATUS "Using .NET host path: ${DOTNET_HOST_PATH}")

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
set(PHYSX_BIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/PhysX/physx/bin/mac.x86_64/checked")
message(STATUS "PhysX bin path: ${PHYSX_BIN_PATH}")

set(PHYSX_COMMON "${PHYSX_BIN_PATH}/libPhysXCommon_static_64.a")
set(PHYSX_CORE "${PHYSX_BIN_PATH}/libPhysX_static_64.a")
set(PHYSX_FOUNDATION "${PHYSX_BIN_PATH}/libPhysXFoundation_static_64.a")
set(PHYSX_CHARACTER_KINEMATIC "${PHYSX_BIN_PATH}/libPhysXCharacterKinematic_static_64.a")
set(PHYSX_COOKING "${PHYSX_BIN_PATH}/libPhysXCooking_static_64.a")
set(PHYSX_PVD "${PHYSX_BIN_PATH}/libPhysXPvdSDK_static_64.a")
set(PHYSX_EXTENSIONS "${PHYSX_BIN_PATH}/libPhysXExtensions_static_64.a")

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
