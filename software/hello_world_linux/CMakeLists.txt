cmake_minimum_required(VERSION 3.16)

project(hello_world_linux LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(hello_world_linux
    src/main.c
)

set_target_properties(hello_world_linux PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_compile_options(hello_world_linux PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

install(TARGETS hello_world_linux RUNTIME DESTINATION bin)
