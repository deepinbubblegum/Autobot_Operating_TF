cmake_minimum_required(VERSION 3.0.0)

add_executable(AutoBuff-GamePro 
    src/main.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(AutoBuff-GamePro server)