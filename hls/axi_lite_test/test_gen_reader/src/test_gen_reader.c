#include "xparameters.h"
#include "xbram.h"

#include "xgenerator.h"
#include "xreader.h"

#include <xbram_hw.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xstatus.h>
#include <stdio.h>


#define GENERATOR_BASEADDR  XPAR_GENERATOR_0_BASEADDR
#define READER_BASEADDR     XPAR_READER_0_BASEADDR

#define DATA_BASEADDR       XPAR_XBRAM_0_BASEADDR

#define N 50

int Init(UINTPTR GenAddr, UINTPTR ReadAddr);
void ReadData(UINTPTR BaseAddr,int n);
int GenReadTest(UINTPTR BaseAddr);


XGenerator GenInst;
XReader    ReaderInst;

int main() {
    int Status;
    
    Status = Init(GENERATOR_BASEADDR, READER_BASEADDR);
    if (Status != XST_SUCCESS) {

        return XST_FAILURE;
    }

    XGenerator_Start(&GenInst);
    XReader_Start(&ReaderInst);

    while (!XReader_IsDone(&ReaderInst)) {}

    Status = GenReadTest(DATA_BASEADDR);
    if (Status != XST_SUCCESS) {
        xil_printf("Data did not match\r\n");
        return XST_FAILURE;
    }
    
    xil_printf("Run Finished\r\n\r\n");
    return XST_SUCCESS;
}


int Init(UINTPTR GenAddr, UINTPTR ReadAddr) {
    int Status;

    Status = XGenerator_Initialize(&GenInst, GenAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("Generator IP Failed to Initialize\r\n");
        return XST_FAILURE;
    }

    Status = XReader_Initialize(&ReaderInst, ReadAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("Reader Failed to Initialize\r\n");
        return XST_FAILURE;
    }

    XReader_Set_out_r(&ReaderInst, DATA_BASEADDR);
    printf("Data base address: %llx\r\n", XReader_Get_out_r(&ReaderInst));

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


int GenReadTest(UINTPTR BaseAddr)
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