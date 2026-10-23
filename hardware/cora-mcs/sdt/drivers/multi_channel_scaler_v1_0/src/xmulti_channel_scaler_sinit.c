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
#include "xmulti_channel_scaler.h"

extern XMulti_channel_scaler_Config XMulti_channel_scaler_ConfigTable[];

#ifdef SDT
XMulti_channel_scaler_Config *XMulti_channel_scaler_LookupConfig(UINTPTR BaseAddress) {
	XMulti_channel_scaler_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XMulti_channel_scaler_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XMulti_channel_scaler_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XMulti_channel_scaler_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMulti_channel_scaler_Initialize(XMulti_channel_scaler *InstancePtr, UINTPTR BaseAddress) {
	XMulti_channel_scaler_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMulti_channel_scaler_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMulti_channel_scaler_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XMulti_channel_scaler_Config *XMulti_channel_scaler_LookupConfig(u16 DeviceId) {
	XMulti_channel_scaler_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMULTI_CHANNEL_SCALER_NUM_INSTANCES; Index++) {
		if (XMulti_channel_scaler_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMulti_channel_scaler_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMulti_channel_scaler_Initialize(XMulti_channel_scaler *InstancePtr, u16 DeviceId) {
	XMulti_channel_scaler_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMulti_channel_scaler_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMulti_channel_scaler_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

