# Find SWIG on the host system
find_package(SWIG REQUIRED)

######################
# BGFX
######################
set(BGFX_CONFIG_MULTITHREADED OFF)

######################
# Assimp
######################
execute_process(
  COMMAND /bin/sh -c "if [ ! -d assimp ]; then git clone https://github.com/assimp/assimp.git; fi"
  COMMAND /bin/sh -c "cd assimp && git fetch && git checkout v5.2.5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/vendor/assimp)

set(ASSIMP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/assimp/include ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/vendor/assimp/include)
set(ASSIMP_LIBRARIES assimp)

######################
# Nlohmann JSON
######################
execute_process(
  COMMAND /bin/sh -c "if [ ! -d assimp ]; then git clone https://github.com/nlohmann/json; fi"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)
set(NLOHMANN_JSON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/json/include")

######################
# GLM
######################
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm")

######################
# SDL2
######################
set(SDL2_STAMP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/em-build/stamp.txt")

add_custom_command(
  OUTPUT ${SDL2_STAMP_FILE}
  COMMAND /bin/sh -c \"if [ ! -d SDL ]\; then git clone https://github.com/libsdl-org/SDL.git\; fi\"
  COMMAND /bin/sh -c \"cd SDL && git fetch && git checkout release-2.28.5\"
  COMMAND /bin/sh -c \"cd SDL && mkdir -p em-build && cd em-build && emconfigure ../configure --host=wasm32-unknown-emscripten --disable-pthreads --disable-assembly --disable-cpuinfo CFLAGS=\"-sUSE_SDL=0 -O3\"\"
  COMMAND /bin/sh -c \"cd SDL && cd em-build && make -j8\"
  COMMAND ${CMAKE_COMMAND} -E touch ${SDL2_STAMP_FILE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)
add_custom_target(SDL2_vendorized ALL DEPENDS ${SDL2_STAMP_FILE})

set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/include")
set(SDL2_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/em-build/build/.libs/libSDL2.a")

######################
# SDL2_mixer
######################
set(SDL2_MIXER_STAMP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/em-build/stamp.txt")
add_custom_command(
  OUTPUT ${SDL2_MIXER_STAMP_FILE}
  COMMAND /bin/sh -c \"if [ ! -d SDL_mixer ]\; then git clone https://github.com/libsdl-org/SDL_mixer.git\; fi\"
  COMMAND /bin/sh -c \"cd SDL_mixer && git fetch && git checkout release-2.6.3\"
  COMMAND /bin/sh -c \"cd SDL_mixer && mkdir -p em-build && cd em-build && emconfigure ../configure --host=wasm32-unknown-emscripten --disable-pthreads --disable-assembly --disable-cpuinfo CFLAGS=\"-sUSE_SDL=0 -O3\"\"
  COMMAND /bin/sh -c \"cd SDL_mixer && cd em-build && make -j8\"
  COMMAND ${CMAKE_COMMAND} -E touch ${SDL2_MIXER_STAMP_FILE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)
add_custom_target(SDL2_mixer_vendorized ALL DEPENDS ${SDL2_MIXER_STAMP_FILE})

set(SDL2_MIXER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/include)
set(SDL2_MIXER_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/em-build/build/.libs/libSDL2_mixer.a)


######################
# IMGUI
######################
add_subdirectory (3rdparty/imgui.cmake)
