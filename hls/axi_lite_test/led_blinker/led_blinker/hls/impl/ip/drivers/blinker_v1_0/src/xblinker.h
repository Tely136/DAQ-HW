// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XBLINKER_H
#define XBLINKER_H

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
#include "xblinker_hw.h"

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
    u64 Test_BaseAddress;
} XBlinker_Config;
#endif

typedef struct {
    u64 Test_BaseAddress;
    u32 IsReady;
} XBlinker;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XBlinker_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XBlinker_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XBlinker_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XBlinker_ReadReg(BaseAddress, RegOffset) \
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
int XBlinker_Initialize(XBlinker *InstancePtr, UINTPTR BaseAddress);
XBlinker_Config* XBlinker_LookupConfig(UINTPTR BaseAddress);
#else
int XBlinker_Initialize(XBlinker *InstancePtr, u16 DeviceId);
XBlinker_Config* XBlinker_LookupConfig(u16 DeviceId);
#endif
int XBlinker_CfgInitialize(XBlinker *InstancePtr, XBlinker_Config *ConfigPtr);
#else
int XBlinker_Initialize(XBlinker *InstancePtr, const char* InstanceName);
int XBlinker_Release(XBlinker *InstancePtr);
#endif


void XBlinker_Set_A(XBlinker *InstancePtr, u32 Data);
u32 XBlinker_Get_A(XBlinker *InstancePtr);
void XBlinker_Set_B(XBlinker *InstancePtr, u32 Data);
u32 XBlinker_Get_B(XBlinker *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
