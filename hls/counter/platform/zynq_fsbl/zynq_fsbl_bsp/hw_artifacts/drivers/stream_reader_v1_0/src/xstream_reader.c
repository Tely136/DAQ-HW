// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xstream_reader.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XStream_reader_CfgInitialize(XStream_reader *InstancePtr, XStream_reader_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XStream_reader_Start(XStream_reader *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_AP_CTRL) & 0x80;
    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XStream_reader_IsDone(XStream_reader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XStream_reader_IsIdle(XStream_reader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XStream_reader_IsReady(XStream_reader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XStream_reader_EnableAutoRestart(XStream_reader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XStream_reader_DisableAutoRestart(XStream_reader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_AP_CTRL, 0);
}

void XStream_reader_Set_data_out(XStream_reader *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_DATA_OUT_DATA, (u32)(Data));
    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_DATA_OUT_DATA + 4, (u32)(Data >> 32));
}

u64 XStream_reader_Get_data_out(XStream_reader *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_DATA_OUT_DATA);
    Data += (u64)XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_DATA_OUT_DATA + 4) << 32;
    return Data;
}

void XStream_reader_Set_n_outer(XStream_reader *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_N_OUTER_DATA, Data);
}

u32 XStream_reader_Get_n_outer(XStream_reader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_N_OUTER_DATA);
    return Data;
}

void XStream_reader_Set_n_inner(XStream_reader *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_N_INNER_DATA, Data);
}

u32 XStream_reader_Get_n_inner(XStream_reader *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_N_INNER_DATA);
    return Data;
}

void XStream_reader_InterruptGlobalEnable(XStream_reader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_GIE, 1);
}

void XStream_reader_InterruptGlobalDisable(XStream_reader *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_GIE, 0);
}

void XStream_reader_InterruptEnable(XStream_reader *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_IER);
    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_IER, Register | Mask);
}

void XStream_reader_InterruptDisable(XStream_reader *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_IER);
    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_IER, Register & (~Mask));
}

void XStream_reader_InterruptClear(XStream_reader *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_reader_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_ISR, Mask);
}

u32 XStream_reader_InterruptGetEnabled(XStream_reader *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_IER);
}

u32 XStream_reader_InterruptGetStatus(XStream_reader *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XStream_reader_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_READER_CONTROL_ADDR_ISR);
}

