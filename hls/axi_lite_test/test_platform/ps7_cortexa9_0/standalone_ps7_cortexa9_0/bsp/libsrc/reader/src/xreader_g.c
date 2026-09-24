#include "xreader.h"

XReader_Config XReader_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,reader-1.0", /* compatible */
		0x40020000 /* reg */
	},
	 {
		 NULL
	}
};