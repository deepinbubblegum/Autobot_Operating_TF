add_executable(autobuff apps.cpp)
target_link_libraries(autobuff server)
target_link_libraries(autobuff adb)