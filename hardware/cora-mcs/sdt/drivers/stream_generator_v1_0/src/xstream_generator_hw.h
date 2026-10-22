// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
// control
// 0x00 : reserved
// 0x04 : reserved
// 0x08 : reserved
// 0x0c : reserved
// 0x10 : Data signal of n_total
//        bit 31~0 - n_total[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of data
//        bit 31~0 - data[31:0] (Read/Write)
// 0x1c : Data signal of data
//        bit 31~0 - data[63:32] (Read/Write)
// 0x20 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XSTREAM_GENERATOR_CONTROL_ADDR_N_TOTAL_DATA 0x10
#define XSTREAM_GENERATOR_CONTROL_BITS_N_TOTAL_DATA 32
#define XSTREAM_GENERATOR_CONTROL_ADDR_DATA_DATA    0x18
#define XSTREAM_GENERATOR_CONTROL_BITS_DATA_DATA    64

