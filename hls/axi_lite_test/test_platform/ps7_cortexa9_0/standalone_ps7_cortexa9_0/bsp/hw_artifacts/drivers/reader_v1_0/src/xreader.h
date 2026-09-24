// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XREADER_H
#define XREADER_H

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
#include "xreader_hw.h"

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
} XReader_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XReader;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XReader_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XReader_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XReader_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XReader_ReadReg(BaseAddress, RegOffset) \
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
int XReader_Initialize(XReader *InstancePtr, UINTPTR BaseAddress);
XReader_Config* XReader_LookupConfig(UINTPTR BaseAddress);
#else
int XReader_Initialize(XReader *InstancePtr, u16 DeviceId);
XReader_Config* XReader_LookupConfig(u16 DeviceId);
#endif
int XReader_CfgInitialize(XReader *InstancePtr, XReader_Config *ConfigPtr);
#else
int XReader_Initialize(XReader *InstancePtr, const char* InstanceName);
int XReader_Release(XReader *InstancePtr);
#endif

void XReader_Start(XReader *InstancePtr);
u32 XReader_IsDone(XReader *InstancePtr);
u32 XReader_IsIdle(XReader *InstancePtr);
u32 XReader_IsReady(XReader *InstancePtr);
void XReader_EnableAutoRestart(XReader *InstancePtr);
void XReader_DisableAutoRestart(XReader *InstancePtr);

void XReader_Set_out_r(XReader *InstancePtr, u64 Data);
u64 XReader_Get_out_r(XReader *InstancePtr);

void XReader_InterruptGlobalEnable(XReader *InstancePtr);
void XReader_InterruptGlobalDisable(XReader *InstancePtr);
void XReader_InterruptEnable(XReader *InstancePtr, u32 Mask);
void XReader_InterruptDisable(XReader *InstancePtr, u32 Mask);
void XReader_InterruptClear(XReader *InstancePtr, u32 Mask);
u32 XReader_InterruptGetEnabled(XReader *InstancePtr);
u32 XReader_InterruptGetStatus(XReader *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
