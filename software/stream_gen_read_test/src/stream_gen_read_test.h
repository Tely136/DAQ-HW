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
// #define DATA_READ_ADDR   0x40003000
#define DATA_READ_ADDR   XPAR_PS7_DDR_0_BASEADDRESS

#define CLK_FREQ    50 // MHz
#define BIN_P       1  // us

#define N_BINS 5
#define N_CLK CLK_FREQ * BIN_P
#define N_TOTAL N_BINS * N_CLK