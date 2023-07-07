

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
  "main.cpp"
)
source_group("Source Files" FILES ${SOURCE_FILES})

################################################################################
# Target
################################################################################
add_library(ZEngine-Editor-Web SHARED ${SOURCE_FILES})
add_dependencies(ZEngine-Editor-Web ZEngine-Core)

################################################################################
# Includes
################################################################################
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/bgfx.cmake/bx/include)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/bgfx.cmake/bimg/include)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/bgfx.cmake/bgfx/tools)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/stb)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/imgui.cmake/imgui)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/iconfontheaders)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/stduuid/include)
target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/inja/single_include)

target_include_directories(ZEngine-Editor-Web PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_include_directories(ZEngine-Editor-Web PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor-Web PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor-Web PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor-Web PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(ZEngine-Editor-Web PRIVATE ${GLFW_INCLUDE_DIRS})

################################################################################
# Linking
################################################################################
target_link_libraries (ZEngine-Editor-Web 
  PRIVATE ZEngine-Core
  PRIVATE IMGUI
  PRIVATE bx
  PRIVATE bimg
  PRIVATE fcpp
  PRIVATE glsl-optimizer
  PRIVATE glslang
  PRIVATE spirv-cross
  PRIVATE spirv-tools
)
