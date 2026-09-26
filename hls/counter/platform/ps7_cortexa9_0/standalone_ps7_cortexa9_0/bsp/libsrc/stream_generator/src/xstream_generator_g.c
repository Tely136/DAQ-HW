#include "xstream_generator.h"

XStream_generator_Config XStream_generator_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,stream-generator-1.0", /* compatible */
		0x40010000 /* reg */
	},
	 {
		 NULL
	}
};