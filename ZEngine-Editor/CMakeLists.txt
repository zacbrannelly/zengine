

################################################################################
# Compiler flags
################################################################################
set (CMAKE_CXX_STANDARD 20)
set(DOTNET_LIBRARY_PATH "/usr/local/share/dotnet/shared/Microsoft.NETCore.App/6.0.14")
set(DOTNET_HOST_PATH "/usr/local/share/dotnet/packs/Microsoft.NETCore.App.Host.osx-arm64/6.0.14/runtimes/osx-arm64/native")

################################################################################
# Source files
################################################################################
set (SOURCE_FILES
  "Builders/MapBuilder.cpp"
  "Builders/MaterialBuilder.cpp"
  "Builders/ShaderBuilder.cpp"
  "Builders/ProjectBuilder.cpp"
  "Controllers/MapController.cpp"
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
  "Exporters/TransformExporter.cpp"
  "Windows/EditorToolbar.cpp"
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
  "Windows/MapViewToolbar.cpp"
  "Dialogs/CreateMapDialog.cpp"
  "Dialogs/CreateShaderDialog.cpp"
  "Dialogs/NewFolderDialog.cpp"
  "Dialogs/CreateMaterialDialog.cpp"
  "Dialogs/BrowserDialog.cpp"
  "Dialogs/UnsavedDialog.cpp"
  "Dialogs/CreateScriptDialog.cpp"
  "Dialogs/ProjectBrowserDialog.cpp"
  "Dialogs/BuildStatusDialog.cpp"
  "Dialogs/CreateProjectDialog.cpp"
  "Popups/AddComponentPopup.cpp"
  "Inspectors/TransformInspector.cpp"
  "Inspectors/CameraInspector.cpp"
  "Inspectors/MeshRendererInspector.cpp"
  "Inspectors/RigidBody3DInspector.cpp"
  "Inspectors/BoxCollider3DInspector.cpp"
  "Inspectors/SphereCollider3DInspector.cpp"
  "Inspectors/CapsuleCollider3DInspector.cpp"
  "Inspectors/Inspector.cpp"
  "Menus/MainMenuBar.cpp"
  "Project/Project.cpp"
  "Editor.cpp"
)
source_group("Source Files" FILES ${SOURCE_FILES})

################################################################################
# Target
################################################################################
add_executable(ZEngine-Editor ${SOURCE_FILES})
add_dependencies(ZEngine-Editor ZEngine-Core)

# Copy the imgui window layout and settings to the build directory
add_custom_command(TARGET ZEngine-Editor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini $<TARGET_FILE_DIR:ZEngine-Editor>/imgui.ini
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/layout.ini $<TARGET_FILE_DIR:ZEngine-Editor>/layout.ini
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/layout.ini
  COMMENT "Copying imgui.ini and layout.ini to build directory"
  VERBATIM
)

# Copy the project templates to the build directory
add_custom_command(TARGET ZEngine-Editor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Builders/templates $<TARGET_FILE_DIR:ZEngine-Editor>/templates
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Project/BaseProject
  COMMENT "Copying BaseProject template to build directory"
  VERBATIM
)

################################################################################
# Includes
################################################################################
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bx/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/tools)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/imgui.cmake/imgui)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/imgui.cmake/guizmo)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/iconfontheaders)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stduuid/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/inja/single_include/inja)

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
  PRIVATE glfw
  PRIVATE SDL2_mixer::SDL2_mixer
)

################################################################################
# Copy dependencies from ZEngine-Core to build directory
################################################################################

add_dependencies(ZEngine-Editor ZEngine-Core-Interop)
add_dependencies(ZEngine-Editor ZEngine-PluginManager-Dotnet)
add_dependencies(ZEngine-Editor ZEngine-Interop-Dotnet)

add_custom_command(TARGET ZEngine-Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/lib/ZEngine.PluginManager $<TARGET_FILE_DIR:ZEngine-Editor>/lib/ZEngine.PluginManager
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/lib/ZEngine.Interop $<TARGET_FILE_DIR:ZEngine-Editor>/lib/ZEngine.Interop
    DEPENDS
      ZEngine-PluginManager-Dotnet
      ZEngine-Interop-Dotnet
      ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/lib/ZEngine.PluginManager
      ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/lib/ZEngine.Interop
    COMMENT "Copying ZEngine.PluginManager and ZEngine.Interop to build directory"
    VERBATIM
)

add_custom_command(TARGET ZEngine-Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ZEngine-Core> $<TARGET_FILE_DIR:ZEngine-Editor>/libZEngine-Core.dylib
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/ZEngine-Core.runtimeconfig.json $<TARGET_FILE_DIR:ZEngine-Editor>/ZEngine-Core.runtimeconfig.json
    DEPENDS
      ZEngine-Core
      ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/libZEngine-Core.dylib
      ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/ZEngine-Core.runtimeconfig.json
    COMMENT "Copying ZEngine-Core library and C# runtime config to build directory"
    VERBATIM
)
