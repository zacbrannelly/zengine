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
    "Component/FreeCamera.cpp"
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
    "Component/DynamicCollider3D.cpp"
    "Component/BoxCollider3D.cpp"
    "Component/SphereCollider3D.cpp"
    "Component/CapsuleCollider3D.cpp"
    "Component/PlaneCollider3D.cpp"
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
    "Rendering/StandardShaders.cpp"
    "Rendering/MaterialFactory.cpp"
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
# Target
################################################################################
add_library(ZEngine-Core SHARED ${SOURCE_FILES})

if (IOS)
    # Make sure the library is built as a framework on iOS.
    set(FRAMEWORK_BUNDLE_IDENTIFIER "com.zacbrannelly.ZEngine-Core")    # <== Set to your framework's bundle identifier (cannot be the same as app bundle identifier)
    set_target_properties(ZEngine-Core PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION A
        MACOSX_FRAMEWORK_IDENTIFIER ${FRAMEWORK_BUNDLE_IDENTIFIER}
        MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/framework.plist.in
        # "current version" in semantic format in Mach-O binary file
        VERSION 1.0.0
        # "compatibility version" in semantic format in Mach-O binary file
        SOVERSION 1.0.0
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET}
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${CODE_SIGN_IDENTITY}
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${DEVICE_FAMILY}
        XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
    )

    # Copy the headers into the framework.
    file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/*.h")
    foreach(HEADER ${HEADER_FILES})
      get_filename_component(DIR ${HEADER} DIRECTORY)
      get_filename_component(FILE_NAME ${HEADER} NAME)
      string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" RELATIVE_DIR ${DIR})
      set(SOURCE_PATH ${HEADER})
      set(DEST_PATH "$<TARGET_FILE_DIR:ZEngine-Core>/Headers/${RELATIVE_DIR}/${FILE_NAME}")
    
      add_custom_command(TARGET ZEngine-Core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:ZEngine-Core>/Headers/${RELATIVE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_PATH} ${DEST_PATH}
        COMMENT "Copying ${FILE_NAME} to framework headers"
      )
    endforeach()
endif()

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
target_include_directories(ZEngine-Core PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_include_directories(ZEngine-Core PRIVATE ${NLOHMANN_JSON_INCLUDE_DIRS})

if (NOT IOS)
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
endif()

# Find PhysX libraries
# TODO: Support other platforms.
if (NOT APPLE)
    message(FATAL_ERROR "Only macOS is supported for now.")
elseif (IOS)
    set(PHYSX_BIN_PATH "${CMAKE_SOURCE_DIR}/3rdparty/PhysX/physx/bin/ios64/checked")
else()
    set(PHYSX_BIN_PATH "${CMAKE_SOURCE_DIR}/3rdparty/PhysX/physx/bin/mac.x86_64/checked")
endif()

message(STATUS "PhysX bin path: ${PHYSX_BIN_PATH}")

set(PHYSX_COMMON "${PHYSX_BIN_PATH}/libPhysXCommon_static_64.a")
set(PHYSX_CORE "${PHYSX_BIN_PATH}/libPhysX_static_64.a")
set(PHYSX_FOUNDATION "${PHYSX_BIN_PATH}/libPhysXFoundation_static_64.a")
set(PHYSX_CHARACTER_KINEMATIC "${PHYSX_BIN_PATH}/libPhysXCharacterKinematic_static_64.a")
set(PHYSX_COOKING "${PHYSX_BIN_PATH}/libPhysXCooking_static_64.a")
set(PHYSX_PVD "${PHYSX_BIN_PATH}/libPhysXPvdSDK_static_64.a")
set(PHYSX_EXTENSIONS "${PHYSX_BIN_PATH}/libPhysXExtensions_static_64.a")

################################################################################
# Linking
################################################################################
target_link_libraries(ZEngine-Core
    PRIVATE bgfx
    PRIVATE bx
    PRIVATE bimg
    PRIVATE ${SDL2_LIBRARIES}
    PRIVATE ${SDL2_IMAGE_LIBRARIES}
    PRIVATE ${SDL2_MIXER_LIBRARIES}
    PRIVATE ${ASSIMP_LIBRARIES}
    PRIVATE ${PHYSX_COMMON}
    PRIVATE ${PHYSX_CORE}
    PRIVATE ${PHYSX_FOUNDATION}
    PRIVATE ${PHYSX_CHARACTER_KINEMATIC}
    PRIVATE ${PHYSX_COOKING}
    PRIVATE ${PHYSX_PVD}
    PRIVATE ${PHYSX_EXTENSIONS}
)

if (IOS)
    target_link_libraries(ZEngine-Core
        PUBLIC ${COREAUDIO}
        PUBLIC ${AVFOUNDATION}
        PUBLIC ${AUDIOTOOLBOX}
        PUBLIC ${COREHAPTICS}
        PUBLIC ${GAMECONTROLLER}
        PUBLIC ${COREMOTION}
        PUBLIC ${COREGRAPHICS}
        PUBLIC ${OPENGLES}
    )
endif()

# Link the dotnet runtime and GLFW if not on iOS
if (NOT IOS)
    target_link_libraries(ZEngine-Core
        PRIVATE glfw
        PRIVATE nlohmann_json::nlohmann_json
        PRIVATE ${DOTNET_CORECLR}
        PRIVATE ${DOTNET_HOST}
    )
endif()

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

if (NOT IOS)
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
endif()

################################################################################
# C# Interop Project
################################################################################

if (NOT IOS)
    set(ZENGINE_INTEROP_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/Lib/ZEngine.Interop)

    # Copy the template project to the output directory
    add_custom_command(TARGET ZEngine-Core-Interop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_PATH} ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
    )

    # Build & publish the interop project using custom target
    add_custom_target(ZEngine-Interop-Dotnet ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_PATH} ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
        COMMAND ${CMAKE_COMMAND} -E env --unset=TARGETNAME ${DOTNET_EXECUTABLE} publish ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/ZEngine.Interop.csproj
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ZEngine-Core-Interop> ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.Interop/libZEngine-Core-Interop.dylib
        COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:ZEngine-Core-Interop>
        DEPENDS ZEngine-Core-Interop
    )

    # Copy the interop dll to the output directory.
    add_custom_command(TARGET ZEngine-Interop-Dotnet POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/bin/Debug/net6.0/publish ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.Interop
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/ZEngine-Core.runtimeconfig.json ${CMAKE_CURRENT_BINARY_DIR}/ZEngine-Core.runtimeconfig.json
        COMMENT "Copy the ZEngine-Interop.dll & its runtime config to the output directory"
        DEPENDS ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
    )
endif()

################################################################################
# C# Plugin Manager Project
################################################################################

if (NOT IOS)
    set(ZENGINE_PLUGIN_MANAGER_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/Lib/ZEngine.PluginManager)
    set(ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/projects/ZEngine.PluginManager)

    # Copy the project to the output directory
    add_custom_command(TARGET ZEngine-Core-Interop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_PATH} ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
    )

    # Build & publish the project using custom target
    add_custom_target(ZEngine-PluginManager-Dotnet ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_PATH} ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
        COMMAND ${CMAKE_COMMAND} -E env --unset=TARGETNAME ${DOTNET_EXECUTABLE} publish ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}/ZEngine.PluginManager.csproj
        DEPENDS ZEngine-Core-Interop
    )

    # Copy the plugin manager dll to the output directory.
    add_custom_command(TARGET ZEngine-PluginManager-Dotnet POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}/bin/Debug/net6.0/publish ${CMAKE_CURRENT_BINARY_DIR}/lib/ZEngine.PluginManager
        COMMENT "Copy the ZEngine.PluginManager.dll to the output directory"
        DEPENDS ${ZENGINE_PLUGIN_MANAGER_PROJECT_BUILD_PATH}
    )
endif()
