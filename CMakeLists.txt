cmake_minimum_required(VERSION 3.10)

project(cse3100)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -g)

add_subdirectory(lab2)
add_subdirectory(ps1)
add_subdirectory(ps2)
add_subdirectory(lab3)
add_subdirectory(ps3)
add_subdirectory(lab4)
add_subdirectory(mt1)
add_subdirectory(lab5)
add_subdirectory(ps4)
add_subdirectory(lab6)
add_subdirectory(lab7)
add_subdirectory(ps5)
add_subdirectory(lab8)
add_subdirectory(ps6)

add_executable(ttest toolbox.c)
