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
// 0x10 : Data signal of data_out
//        bit 31~0 - data_out[31:0] (Read/Write)
// 0x14 : Data signal of data_out
//        bit 31~0 - data_out[63:32] (Read/Write)
// 0x18 : reserved
// 0x1c : Data signal of avg_shot
//        bit 31~0 - avg_shot[31:0] (Read/Write)
// 0x20 : reserved
// 0x24 : Data signal of n_bin
//        bit 31~0 - n_bin[31:0] (Read/Write)
// 0x28 : reserved
// 0x2c : Data signal of n_cycle
//        bit 31~0 - n_cycle[31:0] (Read/Write)
// 0x30 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL       0x00
#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_GIE           0x04
#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_IER           0x08
#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_ISR           0x0c
#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_DATA_OUT_DATA 0x10
#define XMULTI_CHANNEL_SCALER_CONTROL_BITS_DATA_OUT_DATA 64
#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AVG_SHOT_DATA 0x1c
#define XMULTI_CHANNEL_SCALER_CONTROL_BITS_AVG_SHOT_DATA 32
#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_N_BIN_DATA    0x24
#define XMULTI_CHANNEL_SCALER_CONTROL_BITS_N_BIN_DATA    32
#define XMULTI_CHANNEL_SCALER_CONTROL_ADDR_N_CYCLE_DATA  0x2c
#define XMULTI_CHANNEL_SCALER_CONTROL_BITS_N_CYCLE_DATA  32

