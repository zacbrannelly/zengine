################################################################################
# CMake Project Setup
################################################################################
project(IMGUI)
cmake_minimum_required(VERSION 3.25)

################################################################################
# Compiler flags
################################################################################
set (CMAKE_CXX_STANDARD 14)

################################################################################
# Dependencies
################################################################################
find_package(glfw3 3.3 REQUIRED)

################################################################################
# IMGUI
################################################################################
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(SOURCE_FILES
  "${IMGUI_DIR}/imgui_demo.cpp"
  "${IMGUI_DIR}/imgui_draw.cpp"
  "${IMGUI_DIR}/imgui_tables.cpp"
  "${IMGUI_DIR}/imgui_widgets.cpp"
  "${IMGUI_DIR}/imgui.cpp"
  "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
  "guizmo/ImGuizmo.cpp"
)
add_library(IMGUI STATIC ${SOURCE_FILES})

################################################################################
# Includes
################################################################################
target_include_directories(IMGUI PRIVATE ${IMGUI_DIR})
target_include_directories(IMGUI PRIVATE ${GLFW_INCLUDE_DIRS})

################################################################################
# Linking
################################################################################
target_link_libraries(IMGUI PRIVATE glfw)
