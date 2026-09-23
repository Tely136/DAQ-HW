#include "xparameters.h"
#include "xexample.h"
#include "xbram.h"

#include <xbram_hw.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xstatus.h>
#include <stdio.h>


#define WRITER_BASEADDR XPAR_EXAMPLE_0_BASEADDR
#define DATA_BASEADDR XPAR_XBRAM_0_BASEADDR

#define N 50

int WriterInit(UINTPTR BaseAddr);
void ReadData(UINTPTR BaseAddr,int n);
int WriterTest(UINTPTR BaseAddr);

XExample WriterInst;

int main() {
    int Status;

    Status = WriterInit(WRITER_BASEADDR);
    if (Status != XST_SUCCESS)
    {
        xil_printf("Writer failed to initialize\r\n");
        return XST_FAILURE;
    }

    printf("Done: %d\tIdle: %d\tReady: %d\r\n",XExample_IsDone(&WriterInst),XExample_IsIdle(&WriterInst),XExample_IsReady(&WriterInst));

    XExample_Set_a(&WriterInst, DATA_BASEADDR);
    printf("Data base address: %llx\r\n", XExample_Get_a(&WriterInst));

    XExample_Set_N(&WriterInst, N);
    printf("Number of locations: %d\r\n", XExample_Get_N(&WriterInst));


    XExample_Start(&WriterInst);

    // ReadData(DATA_BASEADDR, N);
    
    Status = WriterTest(DATA_BASEADDR);
    if (Status != XST_SUCCESS) {
        xil_printf("Data did not match\r\n");
        return XST_FAILURE;
    }

    xil_printf("Run Finished\r\n\r\n");
    return XST_SUCCESS;
}


int WriterInit(UINTPTR BaseAddr)
{
    int Status;

    Status = XExample_Initialize(&WriterInst, BaseAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("IP Failed to Initialize\r\n");
        return XST_FAILURE;
    }

    return Status;
}

int WriterTest(UINTPTR BaseAddr)
{
    int i;

    for (i=0; i<N; i++) {
        if((int)XBram_ReadReg(BaseAddr, i*4) != i)
        {
            return XST_FAILURE;
        }
    }

    return XST_SUCCESS;
}

void ReadData(UINTPTR BaseAddr, int n)
{
    int val;
    
    for (int i=0; i<n; i++) {
        val = XBram_ReadReg(BaseAddr, i*4);

        printf("Value: %d\r\n", val);
    }
}