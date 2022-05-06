cmake_minimum_required(VERSION 3.0.0)
project(server)

set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/server)

add_library(server STATIC src/server.cpp)
target_include_directories(server PUBLIC include)