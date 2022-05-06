cmake_minimum_required(VERSION 3.0.0)

set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/libserver)

# add_library(libserver SHARED src/server.cpp)
add_library(libserver STATIC src/server.cpp)
target_include_directories(libserver PUBLIC include)