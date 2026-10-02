cmake_minimum_required(VERSION 3.16)
project(bram_test_linux LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(bram_test_linux src/main.c)
set_target_properties(bram_test_linux PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
target_compile_options(bram_test_linux PRIVATE -Wall -Wextra -Wpedantic)
install(TARGETS bram_test_linux RUNTIME DESTINATION bin)
