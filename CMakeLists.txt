cmake_minimum_required(VERSION 3.0.0)
project(AutoBuffOTF VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(AutoBuffOTF)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)