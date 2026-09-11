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
#include "xmcs.h"

extern XMcs_Config XMcs_ConfigTable[];

#ifdef SDT
XMcs_Config *XMcs_LookupConfig(UINTPTR BaseAddress) {
	XMcs_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XMcs_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XMcs_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XMcs_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMcs_Initialize(XMcs *InstancePtr, UINTPTR BaseAddress) {
	XMcs_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMcs_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMcs_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XMcs_Config *XMcs_LookupConfig(u16 DeviceId) {
	XMcs_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMCS_NUM_INSTANCES; Index++) {
		if (XMcs_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMcs_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMcs_Initialize(XMcs *InstancePtr, u16 DeviceId) {
	XMcs_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMcs_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMcs_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

