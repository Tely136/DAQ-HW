#include "xstream_generator.h"
#include "xstream_reader.h"
#include "xparameters.h"

#include "xbram.h"

#include <stdio.h>
#include <xstatus.h>

#include <xil_printf.h>
#include <xil_types.h>


#define GEN_ADDR    XPAR_XSTREAM_GENERATOR_0_BASEADDR
#define READ_ADDR   XPAR_XSTREAM_READER_0_BASEADDR

#define DATA_GEN_ADDR   XPAR_XBRAM_0_BASEADDR
#define DATA_READ_ADDR   0x40003000

#define N_OUTER 5
#define N_INNER 10
#define N_TOTAL 5*10