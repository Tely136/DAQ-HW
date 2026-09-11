// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xmcs.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XMcs_CfgInitialize(XMcs *InstancePtr, XMcs_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XMcs_Set_counts(XMcs *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMcs_WriteReg(InstancePtr->Control_BaseAddress, XMCS_CONTROL_ADDR_COUNTS_DATA, (u32)(Data));
    XMcs_WriteReg(InstancePtr->Control_BaseAddress, XMCS_CONTROL_ADDR_COUNTS_DATA + 4, (u32)(Data >> 32));
}

u64 XMcs_Get_counts(XMcs *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMcs_ReadReg(InstancePtr->Control_BaseAddress, XMCS_CONTROL_ADDR_COUNTS_DATA);
    Data += (u64)XMcs_ReadReg(InstancePtr->Control_BaseAddress, XMCS_CONTROL_ADDR_COUNTS_DATA + 4) << 32;
    return Data;
}

