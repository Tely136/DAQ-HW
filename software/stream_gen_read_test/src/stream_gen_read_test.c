#include "stream_gen_read_test.h"
#include <stdio.h>
#include <xbram_hw.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xstatus.h>
#include <xstream_generator.h>
#include <xstream_reader.h>


int Init(UINTPTR GenAddr, UINTPTR ReaderAddr);
void SetData(UINTPTR BaseAddress, int n, int data[n]);
void ReadData(UINTPTR BaseAddress, int n);
int CheckData(UINTPTR GenAddress, UINTPTR ReadAddress);
int CheckCounts(UINTPTR GenAddress, UINTPTR ReadAddress);

XStream_generator   GenInst;
XStream_reader      ReaderInst;

int main() {
    int Status;

    xil_printf("Started Run\r\n");

    int data[N_TOTAL];
    
    int id = 0;
    for (int i=0; i<N_BINS; i++) {
        for (int j=0; j<N_CLK; j++) {
            data[id] = (j%(i+1)==0);

            id++;
        }
    }

    for (int i=0; i<N_TOTAL; i++) {
        XBram_WriteReg(DATA_GEN_ADDR, i*4, data[i]);
    }

    // ReadData(DATA_GEN_ADDR, N_TOTAL);

    Status = Init(GEN_ADDR, READ_ADDR);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    XStream_generator_Set_n_total(&GenInst,  N_TOTAL);
    XStream_generator_Set_data(&GenInst,     DATA_GEN_ADDR);
    
    XStream_reader_Set_n_outer(&ReaderInst,  N_BINS);
    XStream_reader_Set_n_inner(&ReaderInst,  N_CLK);
    XStream_reader_Set_data_out(&ReaderInst, DATA_READ_ADDR);

    XStream_generator_Start(&GenInst);
    XStream_reader_Start(&ReaderInst);

    while (!XStream_generator_IsDone(&GenInst)) {} 
    while (!XStream_reader_IsDone(&ReaderInst)) {}
    // xil_printf("IP Finished\r\n");

    // printf("Generator   Done: %d\tIdle: %d\tReady: %d\r\n",XStream_generator_IsDone(&GenInst),XStream_generator_IsReady(&GenInst),XStream_generator_IsIdle(&GenInst));
    // printf("Reader      Done: %d\tIdle: %d\tReady: %d\r\n",XStream_reader_IsDone(&ReaderInst),XStream_reader_IsReady(&ReaderInst),XStream_reader_IsIdle(&ReaderInst));

    ReadData(DATA_READ_ADDR, N_BINS);
    Status = CheckCounts(DATA_GEN_ADDR, DATA_READ_ADDR);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    
    xil_printf("Run Finished Successfully\r\n\r\n");
    return XST_SUCCESS;
}


int Init(UINTPTR GenAddr, UINTPTR ReaderAddr) {
    int Status;

    Status = XStream_generator_Initialize(&GenInst, GenAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("Stream generator failed to initialize\r\n");

        return XST_FAILURE;
    }

    Status = XStream_reader_Initialize(&ReaderInst, ReaderAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("Stream reader failed to initialize\r\n");

        return XST_FAILURE;
    }
    
    return XST_SUCCESS;
}

void ReadData(UINTPTR BaseAddress, int n) {
    for (int i=0; i<n; i++) {
        printf("value: %d\r\n", XBram_ReadReg(BaseAddress, i*4));
    }
}

int CheckData(UINTPTR WriteAddress, UINTPTR ReadAddress) {    
    for (int i=0; i<N_BINS; i++) {
            
        if (XBram_ReadReg(WriteAddress, i*4) != XBram_ReadReg(ReadAddress, i*4)) {
            printf("Comparison failed\r\n\r\n");
            return XST_FAILURE;
        }
    }

    return XST_SUCCESS;
}


void SetData(UINTPTR BaseAddress, int n, int data[n]) {
    for (int i=0; i<n; i++) {
        XBram_WriteReg(BaseAddress, i*4, data[i]);
    }
}

int CheckCounts(UINTPTR GenAddress, UINTPTR ReadAddress) {
    int i,j,count_test;
    
    int id = 0;
    for (i=0; i<N_BINS; i++) {
        count_test = 0;
        for (j=0; j<N_CLK; j++) {
            count_test += XBram_ReadReg(GenAddress, id*4);

            id++;
        }

        if (count_test != (int)Xil_In32(ReadAddress + i*4)) {
            xil_printf("Comparison failed\r\n");

            return XST_FAILURE;
        }
    }

    return XST_SUCCESS;
}