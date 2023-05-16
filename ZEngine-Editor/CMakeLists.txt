

################################################################################
# Compiler flags
################################################################################
set (CMAKE_CXX_STANDARD 14)

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
# Source files
################################################################################
set (SOURCE_FILES
  "gui/imgui/imgui.cpp"
  "gui/"
  "ScriptComponentExporter.cpp"
  "GUIImage.cpp"
  "ComponentExporter.cpp"
  "ShaderImporter.cpp"
  "CodeEditor.cpp"
  "AssetImporter.cpp"
  "ShaderBuilder.cpp"
  "AssetExplorer.cpp"
  "InspectorWindow.cpp"
  "GUIDialog.cpp"
  "SceneGraphWindow.cpp"
  "CreateMapDialog.cpp"
  "GUIWindow.cpp"
  "MapBuilder.cpp"
  "CreateShaderDialog.cpp"
  "Directory.cpp"
  "DirectoryWin32.cpp"
  "DirectoryMacOS.cpp"
  "TransformInspector.cpp"
  "CameraInspector.cpp"
  "MapView.cpp"
  "MeshRendererInspector.cpp"
  "GUILibrary.cpp"
  "GUINameGenerator.cpp"
  "File.cpp"
  "ShaderEditor.cpp"
  "Inspector.cpp"
  "NewFolderDialog.cpp"
  "Editor.cpp"
  "AudioSourceExporter.cpp"
  "MeshRendererExporter.cpp"
  "MainMenuBar.cpp"
  "TransformExporter.cpp"
  "CreateMaterialDialog.cpp"
  "Container.cpp"
  "CameraExporter.cpp"
  "GUITextField.cpp"
  "LogWindow.cpp"
  "BrowserDialog.cpp"
  "UnsavedDialog.cpp"
  "MaterialEditor.cpp"
  "MaterialBuilder.cpp"
  "GameView.cpp"
  "CreateScriptDialog.cpp"
)
source_group("Source Files" FILES ${SOURCE_FILES})

################################################################################
# Target
################################################################################
add_executable(ZEngine-Editor ${SOURCE_FILES})

################################################################################
# Includes
################################################################################
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/v8-cmake/v8)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/v8-cmake/v8/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bx/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/tools)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/imgui.cmake/imgui)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/iconfontheaders)

target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(ZEngine-Editor PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor PRIVATE ${GLFW_INCLUDE_DIRS})

################################################################################
# Linking
################################################################################
target_link_libraries (ZEngine-Editor 
  PRIVATE ZEngine-Core
  PRIVATE IMGUI
  PRIVATE bx
  PRIVATE fcpp
  PRIVATE glsl-optimizer
  PRIVATE glslang
  PRIVATE spirv-cross
  PRIVATE spirv-tools
)
