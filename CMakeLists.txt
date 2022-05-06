cmake_minimum_required(VERSION 3.0.0)
project(AutoBuff-GamePro VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# or use MATCHES to see if actual system name 
# Darwin is Apple's system name
if(${CMAKE_SYSTEM_NAME} MATCHES UNIX)
     message("This is a ${CMAKE_SYSTEM_NAME} system")
elseif(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    add_definitions(-DCMAKEMACROSAMPLE="Windows PC")
endif()

add_subdirectory(AutoBuff)
add_subdirectory(server)
