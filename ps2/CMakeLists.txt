add_executable(monthdays calendar.c daysInMonthCounter.c)
add_executable(firstmondays calendar.c firstMondayCounter.c)
add_executable(coins coins.c)
add_executable(pi pi.c)
add_executable(log log.c)
add_executable(lockers lockers.c)

target_compile_options(pi PRIVATE -O2)
