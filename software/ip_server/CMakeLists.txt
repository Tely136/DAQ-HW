cmake_minimum_required(VERSION 3.16)
project(ip_server LANGUAGES C)

add_executable(stream_control_linux
    src/server.c

)
