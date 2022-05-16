add_subdirectory(apps)
add_subdirectory(server)
add_subdirectory(stream)
add_subdirectory(controller)
add_subdirectory(yaml-cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scrcpy-server
     DESTINATION ${CMAKE_BINARY_DIR}/bin)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config
     DESTINATION ${CMAKE_BINARY_DIR}/bin)