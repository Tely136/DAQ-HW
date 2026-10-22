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
#include "xtrigger.h"

extern XTrigger_Config XTrigger_ConfigTable[];

#ifdef SDT
XTrigger_Config *XTrigger_LookupConfig(UINTPTR BaseAddress) {
	XTrigger_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XTrigger_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XTrigger_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XTrigger_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XTrigger_Initialize(XTrigger *InstancePtr, UINTPTR BaseAddress) {
	XTrigger_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XTrigger_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XTrigger_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XTrigger_Config *XTrigger_LookupConfig(u16 DeviceId) {
	XTrigger_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XTRIGGER_NUM_INSTANCES; Index++) {
		if (XTrigger_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XTrigger_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XTrigger_Initialize(XTrigger *InstancePtr, u16 DeviceId) {
	XTrigger_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XTrigger_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XTrigger_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

