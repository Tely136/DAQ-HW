// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#ifdef SDT
#include "xparameters.h"
#endif
#include "xcounter.h"

extern XCounter_Config XCounter_ConfigTable[];

#ifdef SDT
XCounter_Config *XCounter_LookupConfig(UINTPTR BaseAddress) {
	XCounter_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XCounter_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XCounter_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XCounter_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XCounter_Initialize(XCounter *InstancePtr, UINTPTR BaseAddress) {
	XCounter_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XCounter_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XCounter_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XCounter_Config *XCounter_LookupConfig(u16 DeviceId) {
	XCounter_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XCOUNTER_NUM_INSTANCES; Index++) {
		if (XCounter_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XCounter_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XCounter_Initialize(XCounter *InstancePtr, u16 DeviceId) {
	XCounter_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XCounter_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XCounter_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

