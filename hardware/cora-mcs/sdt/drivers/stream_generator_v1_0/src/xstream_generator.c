// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xstream_generator.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XStream_generator_CfgInitialize(XStream_generator *InstancePtr, XStream_generator_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XStream_generator_Set_n_bin(XStream_generator *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_generator_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_GENERATOR_CONTROL_ADDR_N_BIN_DATA, Data);
}

u32 XStream_generator_Get_n_bin(XStream_generator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_generator_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_GENERATOR_CONTROL_ADDR_N_BIN_DATA);
    return Data;
}

void XStream_generator_Set_n_cycle(XStream_generator *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStream_generator_WriteReg(InstancePtr->Control_BaseAddress, XSTREAM_GENERATOR_CONTROL_ADDR_N_CYCLE_DATA, Data);
}

u32 XStream_generator_Get_n_cycle(XStream_generator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStream_generator_ReadReg(InstancePtr->Control_BaseAddress, XSTREAM_GENERATOR_CONTROL_ADDR_N_CYCLE_DATA);
    return Data;
}

