// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xtrigger.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XTrigger_CfgInitialize(XTrigger *InstancePtr, XTrigger_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XTrigger_Set_clk_periods(XTrigger *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_CLK_PERIODS_DATA, Data);
}

u32 XTrigger_Get_clk_periods(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_CLK_PERIODS_DATA);
    return Data;
}

void XTrigger_Set_enable(XTrigger *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_ENABLE_DATA, Data);
}

u32 XTrigger_Get_enable(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_ENABLE_DATA);
    return Data;
}

