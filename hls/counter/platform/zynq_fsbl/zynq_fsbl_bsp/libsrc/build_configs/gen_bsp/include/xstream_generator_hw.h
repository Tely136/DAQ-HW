// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (Read/TOW)
//        bit 1 - ap_ready (Read/TOW)
//        others - reserved
// 0x10 : Data signal of n_total
//        bit 31~0 - n_total[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of data
//        bit 31~0 - data[31:0] (Read/Write)
// 0x1c : Data signal of data
//        bit 31~0 - data[63:32] (Read/Write)
// 0x20 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XSTREAM_GENERATOR_CONTROL_ADDR_AP_CTRL      0x00
#define XSTREAM_GENERATOR_CONTROL_ADDR_GIE          0x04
#define XSTREAM_GENERATOR_CONTROL_ADDR_IER          0x08
#define XSTREAM_GENERATOR_CONTROL_ADDR_ISR          0x0c
#define XSTREAM_GENERATOR_CONTROL_ADDR_N_TOTAL_DATA 0x10
#define XSTREAM_GENERATOR_CONTROL_BITS_N_TOTAL_DATA 32
#define XSTREAM_GENERATOR_CONTROL_ADDR_DATA_DATA    0x18
#define XSTREAM_GENERATOR_CONTROL_BITS_DATA_DATA    64

