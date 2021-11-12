add_executable(dltest dllist.c dltest.c)
add_executable(hmtest dllist.c hashMap.c hmtest.c)

target_link_libraries(dltest pthread)
target_link_libraries(hmtest pthread)

