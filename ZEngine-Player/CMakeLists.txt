################################################################################
# Compiler flags
################################################################################
set (CMAKE_CXX_STANDARD 20)

################################################################################
# Source files
################################################################################
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
target_link_libraries (ZEngine-Player 
  PRIVATE ZEngine-Core
  PRIVATE ${SDL2_MIXER_LIBRARIES}
)

if (NOT IOS)
  target_link_libraries (ZEngine-Player 
    PRIVATE glfw
  )
endif()
