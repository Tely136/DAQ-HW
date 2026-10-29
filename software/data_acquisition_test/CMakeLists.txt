cmake_minimum_required(VERSION 3.16)
project(data_acquisition_test LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(IP_DRIVER_ROOT
    "/mnt/c/Users/tely1/PhD/DAQ-HW/hardware/cora-mcs/sdt/drivers"
    CACHE PATH "Generated HLS driver directory"
)

# improve this to be more generic, and not hardcoded to the specific driver names
set(TRIG_DRIVER "${IP_DRIVER_ROOT}/trigger_v1_0/src") 
set(SRC_DRIVER "${IP_DRIVER_ROOT}/pulse_source_v1_0/src")
set(MCS_DRIVER "${IP_DRIVER_ROOT}/multi_channel_scaler_v1_0/src")

add_executable(data_acquisition_test
    src/main.c
    src/daq.h
    src/daq_tcpip.h
    "${TRIG_DRIVER}/xtrigger.c"
    "${SRC_DRIVER}/xpulse_source.c"
    "${MCS_DRIVER}/xmulti_channel_scaler.c"
)

target_include_directories(data_acquisition_test PRIVATE
    "${TRIG_DRIVER}"
    "${SRC_DRIVER}"
    "${MCS_DRIVER}"
)

target_link_libraries(data_acquisition_test m)