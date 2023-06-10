

################################################################################
# Compiler flags
################################################################################
set (CMAKE_CXX_STANDARD 20)

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
add_dependencies(ZEngine-Editor ZEngine-Core)

# Copy the Assets folder to the build directory.
add_custom_command(TARGET ZEngine-Editor POST_BUILD
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

################################################################################
# Includes
################################################################################
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bx/include)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/tools)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/imgui.cmake/imgui)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/iconfontheaders)
target_include_directories(ZEngine-Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stduuid/include)


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

################################################################################
# Build the C# scripts
# TODO: Move this to its own CMakeLists.txt file
################################################################################

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/ZEngine-Scripts ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/ZEngine-Interop.dll ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts/ZEngine-Interop.dll
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/ZEngine-Scripts
        ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/ZEngine-Interop.dll
    COMMENT "Copying ZEngine-Scripts to build directory"
    VERBATIM
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts
)

# Build the scripts project using custom target (making sure we have the latest scripts & interop dll)
add_custom_target(ZEngine-Scripts ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/ZEngine-Scripts ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/ZEngine-Interop.dll ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts/ZEngine-Interop.dll
    COMMAND ${DOTNET_EXECUTABLE} build ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts/ZEngine-Scripts.csproj
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts
    DEPENDS
        ZEngine-Editor
        ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts
    COMMENT "Building ZEngine-Scripts"
)

# Copy script DLLs, interop DLLs, and core DLLs to the build directory.
add_custom_command(TARGET ZEngine-Scripts POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts/bin/Debug/net6.0/ZEngine-Scripts.dll ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Scripts/ZEngine-Interop.dll ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Interop.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/libZEngine-Core.dylib ${CMAKE_CURRENT_BINARY_DIR}/libZEngine-Core.dylib
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/libZEngine-Core-Interop.dylib ${CMAKE_CURRENT_BINARY_DIR}/libZEngine-Core-Interop.dylib
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/ZEngine-Interop.runtimeconfig.json ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Interop.runtimeconfig.json
    COMMENT "Copying dependencies to build directory"
    VERBATIM
)
