cmake_minimum_required (VERSION 3.0)
project (ZEngine)

# Find dotnet CLI tool
# TODO: Fix this when building for iOS, since this will be required for scripting support.
if (NOT IOS)
  find_program(DOTNET_EXECUTABLE NAMES dotnet)
  if(NOT DOTNET_EXECUTABLE)
    message(FATAL_ERROR "Check for dotnet Program: not found")
  else()
    message(STATUS "Found dotnet Program: ${DOTNET_EXECUTABLE}")
  endif()
endif()

# Include platform specific configurations
if (IOS)
  include(iOS.cmake)
elseif (WIN32)
  include(Windows.cmake)
elseif (UNIX AND NOT APPLE)
  include(Linux.cmake)
elseif (APPLE)
  include(MacOS.cmake)
endif()

# BGFX (and shaderc)
add_subdirectory (3rdparty/bgfx.cmake)
set_property(TARGET shaderc PROPERTY ENABLE_EXPORTS 1)

###
# ZEngine
###

# Core library
add_subdirectory (ZEngine-Core)

if (NOT IOS)
  # Editor (mobile not supported)
  add_subdirectory (ZEngine-Editor)
else()
  # Make sure the SDL libraries are built before the editor
  add_dependencies(ZEngine-Core SDL2_vendorized)
  add_dependencies(ZEngine-Core SDL2_image_vendorized)
  add_dependencies(ZEngine-Core SDL2_mixer_vendorized)
endif()

# Player
add_subdirectory (ZEngine-Player)
