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
#include "xreader.h"

extern XReader_Config XReader_ConfigTable[];

#ifdef SDT
XReader_Config *XReader_LookupConfig(UINTPTR BaseAddress) {
	XReader_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XReader_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XReader_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XReader_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XReader_Initialize(XReader *InstancePtr, UINTPTR BaseAddress) {
	XReader_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XReader_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XReader_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XReader_Config *XReader_LookupConfig(u16 DeviceId) {
	XReader_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XREADER_NUM_INSTANCES; Index++) {
		if (XReader_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XReader_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XReader_Initialize(XReader *InstancePtr, u16 DeviceId) {
	XReader_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XReader_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XReader_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

