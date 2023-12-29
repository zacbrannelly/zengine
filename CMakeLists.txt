cmake_minimum_required (VERSION 3.5)
project (ZEngine)

# Policies for SWIG to work correctly.
# TODO: Write better documentation for this.
if (POLICY CMP0122)
    cmake_policy(SET CMP0122 NEW)
endif()

if (POLICY CMP0078)
    cmake_policy(SET CMP0078 NEW)
endif()

if (POLICY CMP0086)
    cmake_policy(SET CMP0086 NEW)
endif()

# Include SWIG module (for generating C# code).
include(UseSWIG)

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
  # Editor (mobile not supported for obvious reasons)
  add_subdirectory (ZEngine-Editor)
endif()

# Player
add_subdirectory (ZEngine-Player)
