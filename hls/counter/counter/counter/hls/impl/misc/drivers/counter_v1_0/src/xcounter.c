// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xcounter.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XCounter_CfgInitialize(XCounter *InstancePtr, XCounter_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XCounter_Start(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_AP_CTRL) & 0x80;
    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XCounter_IsDone(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XCounter_IsIdle(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XCounter_IsReady(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XCounter_EnableAutoRestart(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XCounter_DisableAutoRestart(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_AP_CTRL, 0);
}

void XCounter_Set_data_out(XCounter *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_DATA_OUT_DATA, (u32)(Data));
    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_DATA_OUT_DATA + 4, (u32)(Data >> 32));
}

u64 XCounter_Get_data_out(XCounter *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_DATA_OUT_DATA);
    Data += (u64)XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_DATA_OUT_DATA + 4) << 32;
    return Data;
}

void XCounter_Set_n_outer(XCounter *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_N_OUTER_DATA, Data);
}

u32 XCounter_Get_n_outer(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_N_OUTER_DATA);
    return Data;
}

void XCounter_Set_n_inner(XCounter *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_N_INNER_DATA, Data);
}

u32 XCounter_Get_n_inner(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_N_INNER_DATA);
    return Data;
}

void XCounter_InterruptGlobalEnable(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_GIE, 1);
}

void XCounter_InterruptGlobalDisable(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_GIE, 0);
}

void XCounter_InterruptEnable(XCounter *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_IER);
    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_IER, Register | Mask);
}

void XCounter_InterruptDisable(XCounter *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_IER);
    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_IER, Register & (~Mask));
}

void XCounter_InterruptClear(XCounter *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_ISR, Mask);
}

u32 XCounter_InterruptGetEnabled(XCounter *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_IER);
}

u32 XCounter_InterruptGetStatus(XCounter *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XCounter_ReadReg(InstancePtr->Control_BaseAddress, XCOUNTER_CONTROL_ADDR_ISR);
}

