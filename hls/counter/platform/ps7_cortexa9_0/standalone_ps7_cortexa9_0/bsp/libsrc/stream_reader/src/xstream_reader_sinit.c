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
#include "xstream_reader.h"

extern XStream_reader_Config XStream_reader_ConfigTable[];

#ifdef SDT
XStream_reader_Config *XStream_reader_LookupConfig(UINTPTR BaseAddress) {
	XStream_reader_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XStream_reader_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XStream_reader_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XStream_reader_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XStream_reader_Initialize(XStream_reader *InstancePtr, UINTPTR BaseAddress) {
	XStream_reader_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XStream_reader_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XStream_reader_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XStream_reader_Config *XStream_reader_LookupConfig(u16 DeviceId) {
	XStream_reader_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XSTREAM_READER_NUM_INSTANCES; Index++) {
		if (XStream_reader_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XStream_reader_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XStream_reader_Initialize(XStream_reader *InstancePtr, u16 DeviceId) {
	XStream_reader_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XStream_reader_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XStream_reader_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

