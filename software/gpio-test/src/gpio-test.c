#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

#define LED 0x01   /* Assumes bit 0 of GPIO is connected to an LED  */
#define	XGPIO_AXI_BASEADDRESS	XPAR_XGPIO_0_BASEADDR
#define LED_DELAY     10000000
#define LED_CHANNEL 2

XGpio Gpio; /* The Instance of the GPIO Driver */

int main(void) {
    int Status;

    volatile int Delay;

	/* Initialize the GPIO driver */

	Status = XGpio_Initialize(&Gpio, XGPIO_AXI_BASEADDRESS);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&Gpio, LED_CHANNEL, ~LED);

	/* Loop forever blinking the LED */

	while (1) {
		/* Set the LED to High */
		XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, LED);

		/* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY; Delay++);

		/* Clear the LED bit */
		XGpio_DiscreteClear(&Gpio, LED_CHANNEL, LED);

		/* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY; Delay++);
	}
}