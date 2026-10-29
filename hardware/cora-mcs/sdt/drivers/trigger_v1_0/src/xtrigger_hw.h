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
// 0x10 : Data signal of clk_periods
//        bit 31~0 - clk_periods[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of enable
//        bit 0  - enable[0] (Read/Write)
//        others - reserved
// 0x1c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XTRIGGER_CONTROL_ADDR_CLK_PERIODS_DATA 0x10
#define XTRIGGER_CONTROL_BITS_CLK_PERIODS_DATA 32
#define XTRIGGER_CONTROL_ADDR_ENABLE_DATA      0x18
#define XTRIGGER_CONTROL_BITS_ENABLE_DATA      1

