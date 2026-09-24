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
#include "xgenerator.h"

extern XGenerator_Config XGenerator_ConfigTable[];

#ifdef SDT
XGenerator_Config *XGenerator_LookupConfig(UINTPTR BaseAddress) {
	XGenerator_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XGenerator_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XGenerator_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XGenerator_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XGenerator_Initialize(XGenerator *InstancePtr, UINTPTR BaseAddress) {
	XGenerator_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XGenerator_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XGenerator_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XGenerator_Config *XGenerator_LookupConfig(u16 DeviceId) {
	XGenerator_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XGENERATOR_NUM_INSTANCES; Index++) {
		if (XGenerator_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XGenerator_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XGenerator_Initialize(XGenerator *InstancePtr, u16 DeviceId) {
	XGenerator_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XGenerator_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XGenerator_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

