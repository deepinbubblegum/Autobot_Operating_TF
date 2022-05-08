add_executable(apps apps.cpp)
target_link_libraries(apps server)
install(TARGETS apps DESTINATION bin)