add_executable(prog1 prog1.c buffer.c)
add_executable(prog2 prog2.c buffer.c)

target_link_libraries(prog1 pthread)
target_link_libraries(prog2 pthread)

if(NOT APPLE)
    target_link_libraries(prog1 rt)
    target_link_libraries(prog2 rt)
endif()
