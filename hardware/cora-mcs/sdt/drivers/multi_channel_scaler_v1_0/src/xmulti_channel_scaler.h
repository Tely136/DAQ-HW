// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XMULTI_CHANNEL_SCALER_H
#define XMULTI_CHANNEL_SCALER_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xmulti_channel_scaler_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Control_BaseAddress;
} XMulti_channel_scaler_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XMulti_channel_scaler;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XMulti_channel_scaler_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XMulti_channel_scaler_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XMulti_channel_scaler_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XMulti_channel_scaler_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XMulti_channel_scaler_Initialize(XMulti_channel_scaler *InstancePtr, UINTPTR BaseAddress);
XMulti_channel_scaler_Config* XMulti_channel_scaler_LookupConfig(UINTPTR BaseAddress);
#else
int XMulti_channel_scaler_Initialize(XMulti_channel_scaler *InstancePtr, u16 DeviceId);
XMulti_channel_scaler_Config* XMulti_channel_scaler_LookupConfig(u16 DeviceId);
#endif
int XMulti_channel_scaler_CfgInitialize(XMulti_channel_scaler *InstancePtr, XMulti_channel_scaler_Config *ConfigPtr);
#else
int XMulti_channel_scaler_Initialize(XMulti_channel_scaler *InstancePtr, const char* InstanceName);
int XMulti_channel_scaler_Release(XMulti_channel_scaler *InstancePtr);
#endif

void XMulti_channel_scaler_Start(XMulti_channel_scaler *InstancePtr);
u32 XMulti_channel_scaler_IsDone(XMulti_channel_scaler *InstancePtr);
u32 XMulti_channel_scaler_IsIdle(XMulti_channel_scaler *InstancePtr);
u32 XMulti_channel_scaler_IsReady(XMulti_channel_scaler *InstancePtr);
void XMulti_channel_scaler_EnableAutoRestart(XMulti_channel_scaler *InstancePtr);
void XMulti_channel_scaler_DisableAutoRestart(XMulti_channel_scaler *InstancePtr);

void XMulti_channel_scaler_Set_data_out(XMulti_channel_scaler *InstancePtr, u64 Data);
u64 XMulti_channel_scaler_Get_data_out(XMulti_channel_scaler *InstancePtr);
void XMulti_channel_scaler_Set_avg_shot(XMulti_channel_scaler *InstancePtr, u32 Data);
u32 XMulti_channel_scaler_Get_avg_shot(XMulti_channel_scaler *InstancePtr);
void XMulti_channel_scaler_Set_n_bin(XMulti_channel_scaler *InstancePtr, u32 Data);
u32 XMulti_channel_scaler_Get_n_bin(XMulti_channel_scaler *InstancePtr);
void XMulti_channel_scaler_Set_n_cycle(XMulti_channel_scaler *InstancePtr, u32 Data);
u32 XMulti_channel_scaler_Get_n_cycle(XMulti_channel_scaler *InstancePtr);

void XMulti_channel_scaler_InterruptGlobalEnable(XMulti_channel_scaler *InstancePtr);
void XMulti_channel_scaler_InterruptGlobalDisable(XMulti_channel_scaler *InstancePtr);
void XMulti_channel_scaler_InterruptEnable(XMulti_channel_scaler *InstancePtr, u32 Mask);
void XMulti_channel_scaler_InterruptDisable(XMulti_channel_scaler *InstancePtr, u32 Mask);
void XMulti_channel_scaler_InterruptClear(XMulti_channel_scaler *InstancePtr, u32 Mask);
u32 XMulti_channel_scaler_InterruptGetEnabled(XMulti_channel_scaler *InstancePtr);
u32 XMulti_channel_scaler_InterruptGetStatus(XMulti_channel_scaler *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
