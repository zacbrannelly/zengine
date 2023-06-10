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
add_executable (ZEngine-Player main.cpp)

################################################################################
# Includes
################################################################################
target_include_directories(ZEngine-Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories(ZEngine-Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb)
target_include_directories(ZEngine-Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stduuid/include)
target_include_directories(ZEngine-Player PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${GLFW_INCLUDE_DIRS})

################################################################################
# Linking
################################################################################
target_link_libraries (ZEngine-Player ZEngine-Core)
