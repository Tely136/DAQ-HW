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
#include "xblinker.h"

extern XBlinker_Config XBlinker_ConfigTable[];

#ifdef SDT
XBlinker_Config *XBlinker_LookupConfig(UINTPTR BaseAddress) {
	XBlinker_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XBlinker_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XBlinker_ConfigTable[Index].Test_BaseAddress == BaseAddress) {
			ConfigPtr = &XBlinker_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XBlinker_Initialize(XBlinker *InstancePtr, UINTPTR BaseAddress) {
	XBlinker_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XBlinker_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XBlinker_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XBlinker_Config *XBlinker_LookupConfig(u16 DeviceId) {
	XBlinker_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XBLINKER_NUM_INSTANCES; Index++) {
		if (XBlinker_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XBlinker_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XBlinker_Initialize(XBlinker *InstancePtr, u16 DeviceId) {
	XBlinker_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XBlinker_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XBlinker_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

