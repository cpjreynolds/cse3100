add_executable(matrix main.c matrix.c)

target_link_libraries(matrix pthread)

if (NOT APPLE)
    target_link_libraries(matrix rt)
endif()
