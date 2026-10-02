cmake_minimum_required(VERSION 3.16)
project(stream_control_linux LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(IP_DRIVER_ROOT
    "/home/tely1/yocto/hardware/cora-mcs/sdt/drivers"
    CACHE PATH "Generated HLS driver directory"
)

set(GEN_DRIVER "${IP_DRIVER_ROOT}/stream_generator_v1_0/src")
set(READ_DRIVER "${IP_DRIVER_ROOT}/stream_reader_v1_0/src")

add_executable(stream_control_linux
    src/main.c
    "${GEN_DRIVER}/xstream_generator.c"
    "${READ_DRIVER}/xstream_reader.c"
)

target_include_directories(stream_control_linux PRIVATE
    "${GEN_DRIVER}"
    "${READ_DRIVER}"
)