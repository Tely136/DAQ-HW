// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xmulti_channel_scaler.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XMulti_channel_scaler_CfgInitialize(XMulti_channel_scaler *InstancePtr, XMulti_channel_scaler_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XMulti_channel_scaler_Start(XMulti_channel_scaler *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL) & 0x80;
    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XMulti_channel_scaler_IsDone(XMulti_channel_scaler *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XMulti_channel_scaler_IsIdle(XMulti_channel_scaler *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XMulti_channel_scaler_IsReady(XMulti_channel_scaler *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XMulti_channel_scaler_EnableAutoRestart(XMulti_channel_scaler *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XMulti_channel_scaler_DisableAutoRestart(XMulti_channel_scaler *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AP_CTRL, 0);
}

void XMulti_channel_scaler_Set_data_out(XMulti_channel_scaler *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_DATA_OUT_DATA, (u32)(Data));
    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_DATA_OUT_DATA + 4, (u32)(Data >> 32));
}

u64 XMulti_channel_scaler_Get_data_out(XMulti_channel_scaler *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_DATA_OUT_DATA);
    Data += (u64)XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_DATA_OUT_DATA + 4) << 32;
    return Data;
}

void XMulti_channel_scaler_Set_avg_shot(XMulti_channel_scaler *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AVG_SHOT_DATA, Data);
}

u32 XMulti_channel_scaler_Get_avg_shot(XMulti_channel_scaler *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_AVG_SHOT_DATA);
    return Data;
}

void XMulti_channel_scaler_Set_n_bin(XMulti_channel_scaler *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_N_BIN_DATA, Data);
}

u32 XMulti_channel_scaler_Get_n_bin(XMulti_channel_scaler *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_N_BIN_DATA);
    return Data;
}

void XMulti_channel_scaler_Set_n_cycle(XMulti_channel_scaler *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_N_CYCLE_DATA, Data);
}

u32 XMulti_channel_scaler_Get_n_cycle(XMulti_channel_scaler *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_N_CYCLE_DATA);
    return Data;
}

void XMulti_channel_scaler_InterruptGlobalEnable(XMulti_channel_scaler *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_GIE, 1);
}

void XMulti_channel_scaler_InterruptGlobalDisable(XMulti_channel_scaler *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_GIE, 0);
}

void XMulti_channel_scaler_InterruptEnable(XMulti_channel_scaler *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_IER);
    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_IER, Register | Mask);
}

void XMulti_channel_scaler_InterruptDisable(XMulti_channel_scaler *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_IER);
    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_IER, Register & (~Mask));
}

void XMulti_channel_scaler_InterruptClear(XMulti_channel_scaler *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMulti_channel_scaler_WriteReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_ISR, Mask);
}

u32 XMulti_channel_scaler_InterruptGetEnabled(XMulti_channel_scaler *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_IER);
}

u32 XMulti_channel_scaler_InterruptGetStatus(XMulti_channel_scaler *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMulti_channel_scaler_ReadReg(InstancePtr->Control_BaseAddress, XMULTI_CHANNEL_SCALER_CONTROL_ADDR_ISR);
}

