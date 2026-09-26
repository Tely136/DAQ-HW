// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XSTREAM_GENERATOR_H
#define XSTREAM_GENERATOR_H

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
#include "xstream_generator_hw.h"

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
} XStream_generator_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XStream_generator;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XStream_generator_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XStream_generator_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XStream_generator_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XStream_generator_ReadReg(BaseAddress, RegOffset) \
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
int XStream_generator_Initialize(XStream_generator *InstancePtr, UINTPTR BaseAddress);
XStream_generator_Config* XStream_generator_LookupConfig(UINTPTR BaseAddress);
#else
int XStream_generator_Initialize(XStream_generator *InstancePtr, u16 DeviceId);
XStream_generator_Config* XStream_generator_LookupConfig(u16 DeviceId);
#endif
int XStream_generator_CfgInitialize(XStream_generator *InstancePtr, XStream_generator_Config *ConfigPtr);
#else
int XStream_generator_Initialize(XStream_generator *InstancePtr, const char* InstanceName);
int XStream_generator_Release(XStream_generator *InstancePtr);
#endif

void XStream_generator_Start(XStream_generator *InstancePtr);
u32 XStream_generator_IsDone(XStream_generator *InstancePtr);
u32 XStream_generator_IsIdle(XStream_generator *InstancePtr);
u32 XStream_generator_IsReady(XStream_generator *InstancePtr);
void XStream_generator_EnableAutoRestart(XStream_generator *InstancePtr);
void XStream_generator_DisableAutoRestart(XStream_generator *InstancePtr);

void XStream_generator_Set_n_total(XStream_generator *InstancePtr, u32 Data);
u32 XStream_generator_Get_n_total(XStream_generator *InstancePtr);
void XStream_generator_Set_data(XStream_generator *InstancePtr, u64 Data);
u64 XStream_generator_Get_data(XStream_generator *InstancePtr);

void XStream_generator_InterruptGlobalEnable(XStream_generator *InstancePtr);
void XStream_generator_InterruptGlobalDisable(XStream_generator *InstancePtr);
void XStream_generator_InterruptEnable(XStream_generator *InstancePtr, u32 Mask);
void XStream_generator_InterruptDisable(XStream_generator *InstancePtr, u32 Mask);
void XStream_generator_InterruptClear(XStream_generator *InstancePtr, u32 Mask);
u32 XStream_generator_InterruptGetEnabled(XStream_generator *InstancePtr);
u32 XStream_generator_InterruptGetStatus(XStream_generator *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
