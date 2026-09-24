// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xgenerator.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XGenerator_CfgInitialize(XGenerator *InstancePtr, XGenerator_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XGenerator_Start(XGenerator *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_AP_CTRL) & 0x80;
    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XGenerator_IsDone(XGenerator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XGenerator_IsIdle(XGenerator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XGenerator_IsReady(XGenerator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XGenerator_EnableAutoRestart(XGenerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XGenerator_DisableAutoRestart(XGenerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_AP_CTRL, 0);
}

void XGenerator_InterruptGlobalEnable(XGenerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_GIE, 1);
}

void XGenerator_InterruptGlobalDisable(XGenerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_GIE, 0);
}

void XGenerator_InterruptEnable(XGenerator *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_IER);
    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_IER, Register | Mask);
}

void XGenerator_InterruptDisable(XGenerator *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_IER);
    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_IER, Register & (~Mask));
}

void XGenerator_InterruptClear(XGenerator *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGenerator_WriteReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_ISR, Mask);
}

u32 XGenerator_InterruptGetEnabled(XGenerator *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_IER);
}

u32 XGenerator_InterruptGetStatus(XGenerator *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XGenerator_ReadReg(InstancePtr->Control_BaseAddress, XGENERATOR_CONTROL_ADDR_ISR);
}

