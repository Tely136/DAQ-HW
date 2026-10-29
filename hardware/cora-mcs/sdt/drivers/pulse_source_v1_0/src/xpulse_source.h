// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XPULSE_SOURCE_H
#define XPULSE_SOURCE_H

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
#include "xpulse_source_hw.h"

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
} XPulse_source_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XPulse_source;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPulse_source_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPulse_source_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPulse_source_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPulse_source_ReadReg(BaseAddress, RegOffset) \
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
int XPulse_source_Initialize(XPulse_source *InstancePtr, UINTPTR BaseAddress);
XPulse_source_Config* XPulse_source_LookupConfig(UINTPTR BaseAddress);
#else
int XPulse_source_Initialize(XPulse_source *InstancePtr, u16 DeviceId);
XPulse_source_Config* XPulse_source_LookupConfig(u16 DeviceId);
#endif
int XPulse_source_CfgInitialize(XPulse_source *InstancePtr, XPulse_source_Config *ConfigPtr);
#else
int XPulse_source_Initialize(XPulse_source *InstancePtr, const char* InstanceName);
int XPulse_source_Release(XPulse_source *InstancePtr);
#endif


void XPulse_source_Set_clk_periods(XPulse_source *InstancePtr, u32 Data);
u32 XPulse_source_Get_clk_periods(XPulse_source *InstancePtr);
void XPulse_source_Set_enable(XPulse_source *InstancePtr, u32 Data);
u32 XPulse_source_Get_enable(XPulse_source *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
