cmake_minimum_required(VERSION 3.0.0)
project(server)

set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

include_directories(${server_SOURCE_DIR}/include)
add_library(server SHARED src/server.cpp)
