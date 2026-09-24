#include "xgenerator.h"

XGenerator_Config XGenerator_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,generator-1.0", /* compatible */
		0x40010000 /* reg */
	},
	 {
		 NULL
	}
};