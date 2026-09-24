// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XGENERATOR_H
#define XGENERATOR_H

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
#include "xgenerator_hw.h"

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
} XGenerator_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XGenerator;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XGenerator_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XGenerator_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XGenerator_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XGenerator_ReadReg(BaseAddress, RegOffset) \
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
int XGenerator_Initialize(XGenerator *InstancePtr, UINTPTR BaseAddress);
XGenerator_Config* XGenerator_LookupConfig(UINTPTR BaseAddress);
#else
int XGenerator_Initialize(XGenerator *InstancePtr, u16 DeviceId);
XGenerator_Config* XGenerator_LookupConfig(u16 DeviceId);
#endif
int XGenerator_CfgInitialize(XGenerator *InstancePtr, XGenerator_Config *ConfigPtr);
#else
int XGenerator_Initialize(XGenerator *InstancePtr, const char* InstanceName);
int XGenerator_Release(XGenerator *InstancePtr);
#endif

void XGenerator_Start(XGenerator *InstancePtr);
u32 XGenerator_IsDone(XGenerator *InstancePtr);
u32 XGenerator_IsIdle(XGenerator *InstancePtr);
u32 XGenerator_IsReady(XGenerator *InstancePtr);
void XGenerator_EnableAutoRestart(XGenerator *InstancePtr);
void XGenerator_DisableAutoRestart(XGenerator *InstancePtr);


void XGenerator_InterruptGlobalEnable(XGenerator *InstancePtr);
void XGenerator_InterruptGlobalDisable(XGenerator *InstancePtr);
void XGenerator_InterruptEnable(XGenerator *InstancePtr, u32 Mask);
void XGenerator_InterruptDisable(XGenerator *InstancePtr, u32 Mask);
void XGenerator_InterruptClear(XGenerator *InstancePtr, u32 Mask);
u32 XGenerator_InterruptGetEnabled(XGenerator *InstancePtr);
u32 XGenerator_InterruptGetStatus(XGenerator *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
