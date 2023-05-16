cmake_minimum_required (VERSION 3.0)
project (ZEngine)

###
# 3rd party libraries (that couldn't be installed as easily)
###

# Google V8
add_subdirectory (3rdparty/v8-cmake)

# Google V8 generates a header file at build time, but it's not generated at configuration time.
# This causes CMake to fail when trying to configure the project.
# To fix this, we create the file at configuration time, and remove it at build time.
set(V8_GENERATED_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/v8-cmake/generated)
set(V8_GENERATED_FILE ${V8_GENERATED_FILES_DIR}/builtins-generated/bytecodes-builtins-list.h)

# Create the file temporarily if it doesn't exist.
if (NOT EXISTS ${V8_GENERATED_FILE})
  execute_process(
    COMMAND mkdir -p ${V8_GENERATED_FILES_DIR}
  )
  file(WRITE ${V8_GENERATED_FILE} "")
endif()

# At build time, we remove the placeholder file if it's empty (so we don't cause V8 to rebuild).
add_custom_target(remove_placeholders
  COMMAND ${CMAKE_COMMAND} -E echo "Checking if file is empty"
  COMMAND python -c "import os; path='${V8_GENERATED_FILE}'; os.remove(path) if os.path.getsize(path) == 0 else None"
  COMMENT "Removing placeholder files for V8 if they are empty"
  VERBATIM
)

# Make sure Google V8 is built after the placeholder files are removed.
add_dependencies(v8_base_without_compiler remove_placeholders)
add_dependencies(v8_compiler remove_placeholders)
add_dependencies(v8_snapshot remove_placeholders)
add_dependencies(v8_inspector remove_placeholders)
add_dependencies(v8_libplatform remove_placeholders)

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
add_subdirectory (ZEngine-Player)
