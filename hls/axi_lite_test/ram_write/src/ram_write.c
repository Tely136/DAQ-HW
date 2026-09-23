/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xbram.h"

#include "xil_io.h"

#include <stdio.h>

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define BRAM_DEVICE_ID		XPAR_XBRAM_0_BASEADDR
#define DDR_BASE_ADDR       XPAR_PS7_DDR_0_BASEADDRESS
#define MY_DDR_BASE_ADDR    XPAR_PS7_DDR_0_BASEADDRESS+0x0100000
/************************** Function Prototypes ******************************/

int BramInitialize(UINTPTR BaseAddress);

void BramReadWrite(UINTPTR BaseAddress);

void DdrReadWrite(UINTPTR BaseAddress);

/************************** Variable Definitions *****************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */
XBram Bram;	/* The Instance of the BRAM Driver */


int main()
{
    int Status;
    
    Status = BramInitialize(BRAM_DEVICE_ID);
	if (Status != XST_SUCCESS ) {
		xil_printf("Bram failed to initialize\r\n");
		return XST_FAILURE;
	}
	xil_printf("Bram initialized\r\n");

    // BramReadWrite(BRAM_DEVICE_ID);

    DdrReadWrite(MY_DDR_BASE_ADDR);

	xil_printf("Run Finished\r\n");
	return XST_SUCCESS;
}

int BramInitialize(UINTPTR BaseAddress)
{
    int Status;
	XBram_Config *ConfigPtr;

    ConfigPtr = XBram_LookupConfig(BaseAddress);
	if (ConfigPtr == (XBram_Config *) NULL) {
		return XST_FAILURE;
	}

	Status = XBram_CfgInitialize(&Bram, ConfigPtr,
				     ConfigPtr->CtrlBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    /*
	 * Execute the BRAM driver selftest.
	 */
	Status = XBram_SelfTest(&Bram, 0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

void BramReadWrite(UINTPTR BaseAddress)
{
    int val;
    int N = 10;
    
    for (int i=0; i<N; i++) {
        XBram_WriteReg(BaseAddress, i*4, i*100);
    }

    for (int i=0; i<N; i++) {
        val = XBram_ReadReg(BaseAddress,i*4);
        printf("value: %d\n",val);
    }

}

void DdrReadWrite(UINTPTR BaseAddress)
{
    int val;
    int N = 10;
    
    for (int i=0; i<N; i++) {
        Xil_Out32(BaseAddress + i*4, i*100);
    }

    for (int i=0; i<N; i++) {
        val = Xil_In32(BaseAddress + i*4);
        printf("value: %d\n",val);
    }
}