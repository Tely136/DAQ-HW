cmake_minimum_required(VERSION 3.16)

project(stream_gen_read_test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(stream_gen_read_test
    src/stream_gen_read_test.cpp
)