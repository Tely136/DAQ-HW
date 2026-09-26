// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
// Tool Version Limit: 2026.06
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XSTREAM_READER_H
#define XSTREAM_READER_H

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
#include "xstream_reader_hw.h"

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
} XStream_reader_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XStream_reader;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XStream_reader_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XStream_reader_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XStream_reader_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XStream_reader_ReadReg(BaseAddress, RegOffset) \
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
int XStream_reader_Initialize(XStream_reader *InstancePtr, UINTPTR BaseAddress);
XStream_reader_Config* XStream_reader_LookupConfig(UINTPTR BaseAddress);
#else
int XStream_reader_Initialize(XStream_reader *InstancePtr, u16 DeviceId);
XStream_reader_Config* XStream_reader_LookupConfig(u16 DeviceId);
#endif
int XStream_reader_CfgInitialize(XStream_reader *InstancePtr, XStream_reader_Config *ConfigPtr);
#else
int XStream_reader_Initialize(XStream_reader *InstancePtr, const char* InstanceName);
int XStream_reader_Release(XStream_reader *InstancePtr);
#endif

void XStream_reader_Start(XStream_reader *InstancePtr);
u32 XStream_reader_IsDone(XStream_reader *InstancePtr);
u32 XStream_reader_IsIdle(XStream_reader *InstancePtr);
u32 XStream_reader_IsReady(XStream_reader *InstancePtr);
void XStream_reader_EnableAutoRestart(XStream_reader *InstancePtr);
void XStream_reader_DisableAutoRestart(XStream_reader *InstancePtr);

void XStream_reader_Set_data_out(XStream_reader *InstancePtr, u64 Data);
u64 XStream_reader_Get_data_out(XStream_reader *InstancePtr);
void XStream_reader_Set_n_outer(XStream_reader *InstancePtr, u32 Data);
u32 XStream_reader_Get_n_outer(XStream_reader *InstancePtr);
void XStream_reader_Set_n_inner(XStream_reader *InstancePtr, u32 Data);
u32 XStream_reader_Get_n_inner(XStream_reader *InstancePtr);

void XStream_reader_InterruptGlobalEnable(XStream_reader *InstancePtr);
void XStream_reader_InterruptGlobalDisable(XStream_reader *InstancePtr);
void XStream_reader_InterruptEnable(XStream_reader *InstancePtr, u32 Mask);
void XStream_reader_InterruptDisable(XStream_reader *InstancePtr, u32 Mask);
void XStream_reader_InterruptClear(XStream_reader *InstancePtr, u32 Mask);
u32 XStream_reader_InterruptGetEnabled(XStream_reader *InstancePtr);
u32 XStream_reader_InterruptGetStatus(XStream_reader *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
