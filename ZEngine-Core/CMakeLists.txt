################################################################################
# Compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 20)

################################################################################
# Source files
################################################################################

set(SOURCE_FILES
    "Assets/Asset.cpp"
    "Assets/AssetCatalog.cpp"
    "Assets/AssetManager.cpp"
    "Assets/Objects/AudioAsset.cpp"
    "Assets/Objects/FontAsset.cpp"
    "Assets/Objects/MapAsset.cpp"
    "Assets/Objects/MaterialAsset.cpp"
    "Assets/Objects/MeshAsset.cpp"
    "Assets/Objects/ModelAsset.cpp"
    "Assets/Objects/CSharpScriptAsset.cpp"
    "Assets/Objects/ShaderAsset.cpp"
    "Assets/Objects/TextureAsset.cpp"
    "Audio/AudioSystem.cpp"
    "Component/AudioSource.cpp"
    "Component/Camera.cpp"
    "Component/Component.cpp"
    "Component/Importers/AudioSourceImporter.cpp"
    "Component/Importers/CameraImporter.cpp"
    "Component/Importers/MeshRendererImporter.cpp"
    "Component/Importers/CSharpScriptComponentImporter.cpp"
    "Component/Importers/TransformImporter.cpp"
    "Component/Importers/RigidBody3DImporter.cpp"
    "Component/Importers/BoxCollider3DImporter.cpp"
    "Component/Importers/SphereCollider3DImporter.cpp"
    "Component/Importers/CapsuleCollider3DImporter.cpp"
    "Component/MeshRenderer.cpp"
    "Component/CSharpScriptComponent.cpp"
    "Component/TestRenderer.cpp"
    "Component/Transform.cpp"
    "Component/RigidBody3D.cpp"
    "Component/Collider3D.cpp"
    "Component/BoxCollider3D.cpp"
    "Component/SphereCollider3D.cpp"
    "Component/CapsuleCollider3D.cpp"
    "Display/Display.cpp"
    "Engine.cpp"
    "Input/InputManager.cpp"
    "Map/Map.cpp"
    "Map/MapManager.cpp"
    "Map/Objects/Entity.cpp"
    "Map/Objects/Primitives/Cube.cpp"
    "Map/Objects/ZObject.cpp"
    "Map/Objects/ObjectType.cpp"
    "Misc/Factory.cpp"
    "Misc/StringUtil.cpp"
    "Physics/Time.cpp"
    "Physics/Physics3DSystem.cpp"
    "Rendering/DynamicIndexBuffer.cpp"
    "Rendering/DynamicVertexBuffer.cpp"
    "Rendering/MetalLayerSetup.mm"
    "Rendering/Graphics.cpp"
    "Rendering/IndexBuffer.cpp"
    "Rendering/Material.cpp"
    "Rendering/Mesh.cpp"
    "Rendering/MeshFactory.cpp"
    "Rendering/Shader.cpp"
    "Rendering/Texture2D.cpp"
    "Rendering/VertexBuffer.cpp"
    "Scripting/CSharp/CSharpScript.cpp"
    "Scripting/CSharp/CSharpScriptSystem.cpp"
    "Scripting/CSharp/AssemblyLoader.cpp"
    "Scripting/CSharp/DotnetRuntime.cpp"
    "Utilities/Directory.cpp"
    "Utilities/DirectoryMacOS.cpp"
    "Utilities/DirectoryWin32.cpp"
    "Utilities/File.cpp"
    "Utilities/FutureHelpers.cpp"
    "GameLoop/GameLoop.cpp"
)
source_group("Source Files" FILES ${SOURCE_FILES})

################################################################################
# 3rdparty
################################################################################
find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(assimp REQUIRED)

################################################################################
# Target
################################################################################
add_library(ZEngine-Core SHARED ${SOURCE_FILES})

################################################################################
# Includes
################################################################################
include_directories("./")

target_include_directories(ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories(ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb)
target_include_directories(ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stduuid/include)
target_include_directories(ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/PhysX/physx/include)

target_include_directories(ZEngine-Core PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${GLFW_INCLUDE_DIRS})

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

# Find PhysX libraries
# TODO: Support other platforms.
set(PHYSX_BIN_PATH "${CMAKE_SOURCE_DIR}/3rdparty/PhysX/physx/bin/mac.x86_64/checked")

find_library(
    PHYSX_COMMON
    NAMES libPhysXCommon_static_64.a
    HINTS "${PHYSX_BIN_PATH}"
)

find_library(
    PHYSX_CORE
    NAMES libPhysX_static_64.a
    HINTS "${PHYSX_BIN_PATH}"
)

find_library(
    PHYSX_FOUNDATION
    NAMES libPhysXFoundation_static_64.a
    HINTS "${PHYSX_BIN_PATH}"
)

find_library(
    PHYSX_CHARACTER_KINEMATIC
    NAMES libPhysXCharacterKinematic_static_64.a
    HINTS "${PHYSX_BIN_PATH}"
)

find_library(
    PHYSX_COOKING
    NAMES libPhysXCooking_static_64.a
    HINTS "${PHYSX_BIN_PATH}"
)

find_library(
    PHYSX_PVD
    NAMES libPhysXPvdSDK_static_64.a
    HINTS "${PHYSX_BIN_PATH}"
)

find_library(
    PHYSX_EXTENSIONS
    NAMES libPhysXExtensions_static_64.a
    HINTS "${PHYSX_BIN_PATH}"
)

# Set RPATH (where to look for shared libraries, i.e. the dotnet runtime)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_RPATH "${DOTNET_LIBRARY_PATH};${DOTNET_HOST_PATH}")

################################################################################
# Linking
################################################################################
target_link_libraries(ZEngine-Core
    PRIVATE bgfx
    PRIVATE bx
    PRIVATE bimg
    PRIVATE SDL2::SDL2
    PRIVATE SDL2_image::SDL2_image
    PRIVATE SDL2_mixer::SDL2_mixer
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE assimp::assimp
    PRIVATE glfw
    PRIVATE ${DOTNET_CORECLR}
    PRIVATE ${DOTNET_HOST}
    PRIVATE ${PHYSX_COMMON}
    PRIVATE ${PHYSX_CORE}
    PRIVATE ${PHYSX_FOUNDATION}
    PRIVATE ${PHYSX_CHARACTER_KINEMATIC}
    PRIVATE ${PHYSX_COOKING}
    PRIVATE ${PHYSX_PVD}
    PRIVATE ${PHYSX_EXTENSIONS}
)

################################################################################
# C# Binding Generation using SWIG
################################################################################

if (POLICY CMP0122)
    cmake_policy(SET CMP0122 NEW)
endif()

if (POLICY CMP0078)
    cmake_policy(SET CMP0078 NEW)
endif()

if (POLICY CMP0086)
    cmake_policy(SET CMP0086 NEW)
endif()

find_package(SWIG REQUIRED)
include(UseSWIG)

set_property(SOURCE Core.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE Core.i PROPERTY COMPILE_OPTIONS
    -namespace ZEngine.Core
    -dllimport lib/ZEngine.Interop/libZEngine-Core-Interop.dylib)

set(ZENGINE_INTEROP_PROJECT_BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/projects/ZEngine.Interop)

swig_add_library(ZEngine-Core-Interop
    TYPE SHARED
    LANGUAGE csharp
    OUTPUT_DIR ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
    SOURCES Core.i
)

set_target_properties(ZEngine-Core-Interop PROPERTIES
    SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
    SWIG_DEPENDS ZEngine-Core
    POSITION_INDEPENDENT_CODE ON
)

# Include the ZEngine-Core include directories
target_include_directories(ZEngine-Core-Interop
    PRIVATE
    "$<TARGET_PROPERTY:ZEngine-Core,INCLUDE_DIRECTORIES>"
)
target_link_libraries(ZEngine-Core-Interop PRIVATE ZEngine-Core)

################################################################################
# C# Interop Project
################################################################################

set(ZENGINE_INTEROP_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/Lib/ZEngine.Interop)

# Copy the template project to the output directory
add_custom_command(TARGET ZEngine-Core-Interop POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_PATH} ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
)

# Build & publish the interop project using custom target
add_custom_target(ZEngine-Interop-Dotnet ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_PATH} ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
    COMMAND ${DOTNET_EXECUTABLE} publish ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/ZEngine.Interop.csproj
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libZEngine-Core-Interop.dylib ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.Interop/libZEngine-Core-Interop.dylib
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/libZEngine-Core-Interop.dylib
    DEPENDS ZEngine-Core-Interop
)

# Copy the interop dll to the output directory.
add_custom_command(TARGET ZEngine-Interop-Dotnet POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/bin/Debug/net6.0/publish ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.Interop
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/ZEngine-Core.runtimeconfig.json ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Core.runtimeconfig.json
    COMMENT "Copy the ZEngine-Interop.dll & its runtime config to the output directory"
    DEPENDS ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
)

################################################################################
# C# Plugin Manager Project
################################################################################

set(ZENGINE_PLUGIN_MANAGER_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/Lib/ZEngine.PluginManager)
set(ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/projects/ZEngine.PluginManager)

# Copy the project to the output directory
add_custom_command(TARGET ZEngine-Core-Interop POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_PATH} ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
)

# Build & publish the project using custom target
add_custom_target(ZEngine-PluginManager-Dotnet ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_PATH} ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
    COMMAND ${DOTNET_EXECUTABLE} publish ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}/ZEngine.PluginManager.csproj
    DEPENDS ZEngine-Core-Interop
)

# Copy the plugin manager dll to the output directory.
add_custom_command(TARGET ZEngine-PluginManager-Dotnet POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}/bin/Debug/net6.0/publish ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.PluginManager
    COMMENT "Copy the ZEngine.PluginManager.dll to the output directory"
    DEPENDS ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
)
