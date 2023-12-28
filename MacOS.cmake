# Find depdendencies using CMake's find_package

find_package(SDL2 REQUIRED COMPONENTS SDL2)
set(SDL2_LIBRARIES SDL2::SDL2)

find_package(SDL2_mixer REQUIRED)
set(SDL2_MIXER_LIBRARIES SDL2_mixer::SDL2_mixer)

find_package(SDL2_image REQUIRED)
set(SDL2_IMAGE_LIBRARIES SDL2_image::SDL2_image)

find_package(assimp REQUIRED)
set(ASSIMP_LIBRARIES assimp::assimp)

find_package(glfw3 3.3 REQUIRED)

find_package(glm REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# ImGui (needed for the editor)
add_subdirectory (3rdparty/imgui.cmake)
