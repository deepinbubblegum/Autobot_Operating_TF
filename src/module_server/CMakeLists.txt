set(MODULE_NAME server)
add_library(${MODULE_NAME} STATIC src/server.cpp)
target_include_directories(${MODULE_NAME} PUBLIC include)