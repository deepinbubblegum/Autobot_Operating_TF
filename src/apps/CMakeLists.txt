add_executable(apps apps.cpp)
target_link_libraries(apps server)
target_link_libraries(apps adb)