// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xreader.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XReader_CfgInitialize(XReader *InstancePtr, XReader_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XReader_Start(XReader *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_AP_CTRL) & 0x80;
    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XReader_IsDone(XReader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XReader_IsIdle(XReader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XReader_IsReady(XReader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XReader_EnableAutoRestart(XReader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XReader_DisableAutoRestart(XReader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_AP_CTRL, 0);
}

void XReader_Set_out_r(XReader *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_OUT_R_DATA, (u32)(Data));
    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_OUT_R_DATA + 4, (u32)(Data >> 32));
}

u64 XReader_Get_out_r(XReader *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_OUT_R_DATA);
    Data += (u64)XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_OUT_R_DATA + 4) << 32;
    return Data;
}

void XReader_InterruptGlobalEnable(XReader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_GIE, 1);
}

void XReader_InterruptGlobalDisable(XReader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_GIE, 0);
}

void XReader_InterruptEnable(XReader *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_IER);
    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_IER, Register | Mask);
}

void XReader_InterruptDisable(XReader *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_IER);
    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_IER, Register & (~Mask));
}

void XReader_InterruptClear(XReader *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReader_WriteReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_ISR, Mask);
}

u32 XReader_InterruptGetEnabled(XReader *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_IER);
}

u32 XReader_InterruptGetStatus(XReader *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XReader_ReadReg(InstancePtr->Control_BaseAddress, XREADER_CONTROL_ADDR_ISR);
}

