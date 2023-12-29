# Find Dotnet CLI tool (using the find_host_program macro provided by the iOS CMake toolchain)
find_host_program(DOTNET_EXECUTABLE NAMES dotnet)
if(NOT DOTNET_EXECUTABLE)
  message(FATAL_ERROR "Check for dotnet Program: not found")
else()
  message(STATUS "Found dotnet Program: ${DOTNET_EXECUTABLE}")
endif()

# Find SWIG on the host system.
find_host_package(SWIG REQUIRED)

set(DEVELOPMENT_PROJECT_NAME "ZEngine")                     # <== Set to your project name, e.g. project.xcodeproj
set(DEVELOPMENT_TEAM_ID "RFFZEYZT36")                       # <== Set to your team ID from Apple
set(CODE_SIGN_IDENTITY "321BE4FC4E46B2522704FAC2B11DC851636BA4B1") # <== Set to your preferred code sign identity, to see list: /usr/bin/env xcrun security find-identity -v -p codesigning
set(DEPLOYMENT_TARGET 13.0)                                  # <== Set your deployment target version of iOS
set(DEVICE_FAMILY "1")                                      # <== Set to "1" to target iPhone, set to "2" to target iPad, set to "1,2" to target both
set(MACOSX_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET})

# Build SDL2 for iOS using Xcode, as a static library using a custom target
add_custom_target(
  SDL2_vendorized
  COMMAND /bin/sh -c \"if [ ! -d SDL ]\; then git clone https://github.com/libsdl-org/SDL.git\; fi\"
  COMMAND /bin/sh -c \"cd SDL && git fetch && git checkout release-2.28.5\"
  COMMAND xcodebuild -project ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/Xcode/SDL/SDL.xcodeproj -scheme \"Static Library-iOS\" -configuration Release -sdk iphoneos -derivedDataPath ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build/iOS
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build/Build/Products/Release-iphoneos/libSDL2.a ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build/iOS/libSDL2.a
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build/iOS/include
  COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/include/* ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build/iOS/include
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)

set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build/iOS/include)
set(SDL2_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL/build/iOS/libSDL2.a)

# Build SDL2_image for iOS using Xcode, as a static library using a custom target
add_custom_target(
  SDL2_image_vendorized
  COMMAND /bin/sh -c \"if [ ! -d SDL_image ]\; then git clone https://github.com/libsdl-org/SDL_image.git\; fi\"
  COMMAND /bin/sh -c \"cd SDL_image && git fetch && git checkout release-2.8.1\"
  COMMAND xcodebuild -project ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/Xcode/SDL_image.xcodeproj -scheme \"Static Library\" -configuration Release -sdk iphoneos -derivedDataPath ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build/iOS
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build/Build/Products/Release-iphoneos/libSDL2_image.a ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build/iOS/libSDL2_image.a
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build/iOS/include
  COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/include/* ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build/iOS/include
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)

set(SDL2_IMAGE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build/iOS/include)
set(SDL2_IMAGE_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_image/build/iOS/libSDL2_image.a)

# Build SDL2_mixer for iOS using Xcode, as a static library using a custom target
add_custom_target(
  SDL2_mixer_vendorized
  COMMAND /bin/sh -c \"if [ ! -d SDL_mixer ]\; then git clone https://github.com/libsdl-org/SDL_mixer.git\; fi\"
  COMMAND /bin/sh -c \"cd SDL_mixer && git fetch && git checkout release-2.6.3\"
  COMMAND xcodebuild -project ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/Xcode/SDL_mixer.xcodeproj -scheme \"Static Library\" -configuration Release -sdk iphoneos -derivedDataPath ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build/iOS
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build/Build/Products/Release-iphoneos/libSDL2_mixer.a ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build/iOS/libSDL2_mixer.a
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build/iOS/include
  COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/include/* ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build/iOS/include
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)

set(SDL2_MIXER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build/iOS/include)
set(SDL2_MIXER_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/SDL_mixer/build/iOS/libSDL2_mixer.a)

# Assimp
execute_process(
  COMMAND /bin/sh -c "if [ ! -d assimp ]; then git clone https://github.com/assimp/assimp.git; fi"
  COMMAND /bin/sh -c "cd assimp && git fetch && git checkout v5.2.5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/vendor/assimp)

set(ASSIMP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vendor/assimp/include)
set(ASSIMP_LIBRARIES assimp)

# TODO: Use CMake to find these libraries
set(GLM_INCLUDE_DIRS "/opt/homebrew/Cellar/glm/0.9.9.8/include")
set(NLOHMANN_JSON_INCLUDE_DIRS "/opt/homebrew/Cellar/nlohmann-json/3.11.2/include")

find_library(COREAUDIO CoreAudio REQUIRED)
find_library(AVFOUNDATION AVFoundation REQUIRED)
find_library(AUDIOTOOLBOX AudioToolbox REQUIRED)
find_library(COREHAPTICS CoreHaptics REQUIRED)
find_library(GAMECONTROLLER GameController REQUIRED)
find_library(COREMOTION CoreMotion REQUIRED)
find_library(COREGRAPHICS CoreGraphics REQUIRED)
find_library(OPENGLES OpenGLES REQUIRED)
