add_executable(pclock pclock.c guess-mt.c)

target_link_libraries(pclock pthread)
