# ==============================================================
# Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
# Tool Version Limit: 2026.06
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
# 
# ==============================================================
proc generate {drv_handle} {
    xdefine_include_file $drv_handle "xparameters.h" "XBlinker" \
        "NUM_INSTANCES" \
        "DEVICE_ID" \
        "C_S_AXI_TEST_BASEADDR" \
        "C_S_AXI_TEST_HIGHADDR"

    xdefine_config_file $drv_handle "xblinker_g.c" "XBlinker" \
        "DEVICE_ID" \
        "C_S_AXI_TEST_BASEADDR"

    xdefine_canonical_xpars $drv_handle "xparameters.h" "XBlinker" \
        "DEVICE_ID" \
        "C_S_AXI_TEST_BASEADDR" \
        "C_S_AXI_TEST_HIGHADDR"
}

