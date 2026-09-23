#include "xexample.h"

XExample_Config XExample_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,example-1.0", /* compatible */
		0x40020000 /* reg */
	},
	 {
		 NULL
	}
};