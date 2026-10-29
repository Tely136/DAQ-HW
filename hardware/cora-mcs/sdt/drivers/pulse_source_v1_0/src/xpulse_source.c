// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xpulse_source.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPulse_source_CfgInitialize(XPulse_source *InstancePtr, XPulse_source_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPulse_source_Set_clk_periods(XPulse_source *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPulse_source_WriteReg(InstancePtr->Control_BaseAddress, XPULSE_SOURCE_CONTROL_ADDR_CLK_PERIODS_DATA, Data);
}

u32 XPulse_source_Get_clk_periods(XPulse_source *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPulse_source_ReadReg(InstancePtr->Control_BaseAddress, XPULSE_SOURCE_CONTROL_ADDR_CLK_PERIODS_DATA);
    return Data;
}

void XPulse_source_Set_enable(XPulse_source *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPulse_source_WriteReg(InstancePtr->Control_BaseAddress, XPULSE_SOURCE_CONTROL_ADDR_ENABLE_DATA, Data);
}

u32 XPulse_source_Get_enable(XPulse_source *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPulse_source_ReadReg(InstancePtr->Control_BaseAddress, XPULSE_SOURCE_CONTROL_ADDR_ENABLE_DATA);
    return Data;
}

