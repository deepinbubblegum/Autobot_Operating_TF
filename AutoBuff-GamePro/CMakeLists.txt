cmake_minimum_required(VERSION 3.0.0)
project(AutoBuff-GamePro VERSION 0.1.0)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable(AutoBuff-GamePro src/main.cpp)

target_link_libraries(AutoBuff-GamePro 
    libserver
)