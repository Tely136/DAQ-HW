// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xblinker.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XBlinker_CfgInitialize(XBlinker *InstancePtr, XBlinker_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Test_BaseAddress = ConfigPtr->Test_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XBlinker_Set_A(XBlinker *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlinker_WriteReg(InstancePtr->Test_BaseAddress, XBLINKER_TEST_ADDR_A_DATA, Data);
}

u32 XBlinker_Get_A(XBlinker *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlinker_ReadReg(InstancePtr->Test_BaseAddress, XBLINKER_TEST_ADDR_A_DATA);
    return Data;
}

void XBlinker_Set_B(XBlinker *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlinker_WriteReg(InstancePtr->Test_BaseAddress, XBLINKER_TEST_ADDR_B_DATA, Data);
}

u32 XBlinker_Get_B(XBlinker *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlinker_ReadReg(InstancePtr->Test_BaseAddress, XBLINKER_TEST_ADDR_B_DATA);
    return Data;
}

