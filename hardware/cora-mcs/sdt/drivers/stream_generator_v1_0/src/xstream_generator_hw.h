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
// 0x10 : Data signal of n_bin
//        bit 31~0 - n_bin[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of n_cycle
//        bit 31~0 - n_cycle[31:0] (Read/Write)
// 0x1c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XSTREAM_GENERATOR_CONTROL_ADDR_N_BIN_DATA   0x10
#define XSTREAM_GENERATOR_CONTROL_BITS_N_BIN_DATA   32
#define XSTREAM_GENERATOR_CONTROL_ADDR_N_CYCLE_DATA 0x18
#define XSTREAM_GENERATOR_CONTROL_BITS_N_CYCLE_DATA 32

