cmake_minimum_required (VERSION 3.0)
project (ZEngine)

# Find dotnet cli
find_program(DOTNET_EXECUTABLE NAMES dotnet)
if(NOT DOTNET_EXECUTABLE)
  message(FATAL_ERROR "Check for dotnet Program: not found")
else()
  message(STATUS "Found dotnet Program: ${DOTNET_EXECUTABLE}")
endif()

###
# 3rd party libraries (that couldn't be installed as easily)
###

# BGFX (and shaderc)
add_subdirectory (3rdparty/bgfx.cmake)
set_property(TARGET shaderc PROPERTY ENABLE_EXPORTS 1)

# ImGui
add_subdirectory (3rdparty/imgui.cmake)

###
# ZEngine
###
add_subdirectory (ZEngine-Core)
add_subdirectory (ZEngine-Editor)
add_subdirectory (ZEngine-Editor-Web/native)
add_subdirectory (ZEngine-Player)
