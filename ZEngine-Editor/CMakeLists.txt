

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
  "UI/imgui/imgui.cpp"
  "UI/GUIImage.cpp"
  "UI/GUIDialog.cpp"
  "UI/GUIWindow.cpp"
  "UI/GUILibrary.cpp"
  "UI/GUINameGenerator.cpp"
  "UI/Container.cpp"
  "UI/GUITextField.cpp"
  "Exporters/AudioSourceExporter.cpp"
  "Exporters/CameraExporter.cpp"
  "Exporters/ComponentExporter.cpp"
  "Exporters/MeshRendererExporter.cpp"
  "Exporters/ScriptComponentExporter.cpp"
  "Exporters/TransformExporter.cpp"
  "Exporters/MapBuilder.cpp"
  "Exporters/MaterialBuilder.cpp"
  "Exporters/ShaderBuilder.cpp"
  "Windows/ShaderImporter.cpp"
  "Windows/CodeEditor.cpp"
  "Windows/AssetImporter.cpp"
  "Windows/AssetExplorer.cpp"
  "Windows/InspectorWindow.cpp"
  "Windows/SceneGraphWindow.cpp"
  "Windows/MapView.cpp"
  "Windows/ShaderEditor.cpp"
  "Windows/LogWindow.cpp"
  "Windows/MaterialEditor.cpp"
  "Windows/GameView.cpp"
  "Dialogs/CreateMapDialog.cpp"
  "Dialogs/CreateShaderDialog.cpp"
  "Dialogs/NewFolderDialog.cpp"
  "Dialogs/CreateMaterialDialog.cpp"
  "Dialogs/BrowserDialog.cpp"
  "Dialogs/UnsavedDialog.cpp"
  "Dialogs/CreateScriptDialog.cpp"
  "Inspectors/TransformInspector.cpp"
  "Inspectors/CameraInspector.cpp"
  "Inspectors/MeshRendererInspector.cpp"
  "Inspectors/Inspector.cpp"
  "Menus/MainMenuBar.cpp"
  "Utilities/Directory.cpp"
  "Utilities/DirectoryWin32.cpp"
  "Utilities/DirectoryMacOS.cpp"
  "Utilities/File.cpp"
  "Editor.cpp"
)
source_group("Source Files" FILES ${SOURCE_FILES})

################################################################################
# Target
################################################################################
add_executable(ZEngine-Editor ${SOURCE_FILES})

# Copy the Assets folder to the build directory.
add_custom_target(ZEngine-Editor-Copy-Assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/catalog.zengine ${CMAKE_CURRENT_BINARY_DIR}/catalog.zengine
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini ${CMAKE_CURRENT_BINARY_DIR}/imgui.ini
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/layout.ini ${CMAKE_CURRENT_BINARY_DIR}/layout.ini
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Assets
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/catalog.zengine
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/layout.ini
  COMMENT "Copying Assets folder to build directory"
  VERBATIM
)
add_dependencies(ZEngine-Editor ZEngine-Editor-Copy-Assets)

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
