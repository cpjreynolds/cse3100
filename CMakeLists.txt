cmake_minimum_required(VERSION 3.10)

project(cse3100)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

add_subdirectory(lab2)
add_subdirectory(ps1)
add_subdirectory(ps2)
add_subdirectory(lab3)
add_subdirectory(ps3)
add_subdirectory(lab4)
add_subdirectory(mt1)

add_executable(ttest toolbox.c)
