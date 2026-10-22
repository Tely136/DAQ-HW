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

void XTrigger_Start(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL) & 0x80;
    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XTrigger_IsDone(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XTrigger_IsIdle(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XTrigger_IsReady(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XTrigger_EnableAutoRestart(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XTrigger_DisableAutoRestart(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL, 0);
}

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

void XTrigger_InterruptGlobalEnable(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_GIE, 1);
}

void XTrigger_InterruptGlobalDisable(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_GIE, 0);
}

void XTrigger_InterruptEnable(XTrigger *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER);
    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER, Register | Mask);
}

void XTrigger_InterruptDisable(XTrigger *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER);
    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER, Register & (~Mask));
}

void XTrigger_InterruptClear(XTrigger *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_ISR, Mask);
}

u32 XTrigger_InterruptGetEnabled(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER);
}

u32 XTrigger_InterruptGetStatus(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_ISR);
}

