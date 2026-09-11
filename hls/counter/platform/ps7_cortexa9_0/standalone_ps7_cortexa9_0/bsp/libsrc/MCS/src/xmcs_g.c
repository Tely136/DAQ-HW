#include "xmcs.h"

XMcs_Config XMcs_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,MCS-1.0", /* compatible */
		0x40000000 /* reg */
	},
	 {
		 NULL
	}
};