################################################################################
# Compiler flags
################################################################################
set (CMAKE_CXX_STANDARD 14)

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
    "Assets/Objects/ScriptAsset.cpp"
    "Assets/Objects/ShaderAsset.cpp"
    "Assets/Objects/TextureAsset.cpp"
    "Audio/AudioSystem.cpp"
    "Component/AudioSource.cpp"
    "Component/Camera.cpp"
    "Component/Component.cpp"
    "Component/Importers/AudioSourceImporter.cpp"
    "Component/Importers/CameraImporter.cpp"
    "Component/Importers/MeshRendererImporter.cpp"
    "Component/Importers/ScriptComponentImporter.cpp"
    "Component/Importers/TransformImporter.cpp"
    "Component/MeshRenderer.cpp"
    "Component/ScriptComponent.cpp"
    "Component/TestRenderer.cpp"
    "Component/Transform.cpp"
    "Display/Display.cpp"
    "Engine.cpp"
    "Input/InputManager.cpp"
    "Input/InputTypes.cpp"
    "Map/Map.cpp"
    "Map/MapManager.cpp"
    "Map/Objects/Entity.cpp"
    "Map/Objects/Primitives/Cube.cpp"
    "Map/Objects/ZObject.cpp"
    "Misc/Factory.cpp"
    "Misc/StringUtil.cpp"
    "Physics/Time.cpp"
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
    "Scripting/IScriptable.cpp"
    "Scripting/Script.cpp"
    "Scripting/ScriptDemo.cpp"
    "Scripting/ScriptSystem.cpp"
    "Scripting/Wrappers/FactoryWrapper.cpp"
    "Scripting/Wrappers/MapManagerWrapper.cpp"
    "Scripting/Wrappers/TimeWrapper.cpp"
    "Scripting/Wrappers/Vec2Wrapper.cpp"
    "Scripting/Wrappers/Vec3Wrapper.cpp"
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
add_library(ZEngine-Core ${SOURCE_FILES})

################################################################################
# Includes
################################################################################
include_directories("./")

target_include_directories (ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/v8-cmake/v8)
target_include_directories (ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/v8-cmake/v8/include)
target_include_directories (ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories (ZEngine-Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb)

target_include_directories(ZEngine-Core PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${GLFW_INCLUDE_DIRS})

################################################################################
# Linking
################################################################################
target_link_libraries(ZEngine-Core
    PRIVATE v8_base_without_compiler
    PRIVATE v8_compiler
    PRIVATE v8_snapshot
    PRIVATE v8_inspector
    PRIVATE v8_libplatform
    PRIVATE bgfx
    PRIVATE bx
    PRIVATE bimg
    PRIVATE SDL2::SDL2
    PRIVATE SDL2_image::SDL2_image
    PRIVATE SDL2_mixer::SDL2_mixer
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE assimp::assimp
    PRIVATE glfw
)
