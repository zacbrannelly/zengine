add_executable (ZEngine-Player main.cpp)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell.html")
target_link_options(ZEngine-Player PRIVATE
  "-sALLOW_MEMORY_GROWTH=1"
  "-sMIN_WEBGL_VERSION=2"
  "-sSAFE_HEAP=1"
  "-lidbfs.js"
)
set_target_properties(ZEngine-Player PROPERTIES OUTPUT_NAME "index")
set_target_properties(ZEngine-Player PROPERTIES SUFFIX ".html")

target_compile_options(ZEngine-Player PRIVATE "--use-port=contrib.glfw3")
target_link_options(ZEngine-Player PRIVATE "--use-port=contrib.glfw3")
