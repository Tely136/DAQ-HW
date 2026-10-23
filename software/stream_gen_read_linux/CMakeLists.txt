cmake_minimum_required(VERSION 3.16)
project(stream_control_linux LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(IP_DRIVER_ROOT
    "/mnt/c/Users/tely1/PhD/DAQ-HW/hardware/cora-mcs/sdt/drivers"
    CACHE PATH "Generated HLS driver directory"
)

# improve this to be more generic, and not hardcoded to the specific driver names
set(TRIG_DRIVER "${IP_DRIVER_ROOT}/trigger_v1_0/src") 
set(GEN_DRIVER "${IP_DRIVER_ROOT}/stream_generator_v1_0/src")
set(MCS_DRIVER "${IP_DRIVER_ROOT}/multi_channel_scaler_v1_0/src")

add_executable(stream_control_linux
    src/main.c
    src/daq.h
    src/daq_tcpip.h
    "${TRIG_DRIVER}/xtrigger.c"
    "${GEN_DRIVER}/xstream_generator.c"
    "${MCS_DRIVER}/xmulti_channel_scaler.c"
)

target_include_directories(stream_control_linux PRIVATE
    "${TRIG_DRIVER}"
    "${GEN_DRIVER}"
    "${MCS_DRIVER}"
)