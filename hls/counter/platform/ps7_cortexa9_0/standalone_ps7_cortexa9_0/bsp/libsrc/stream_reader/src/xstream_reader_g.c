#include "xstream_reader.h"

XStream_reader_Config XStream_reader_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,stream-reader-1.0", /* compatible */
		0x40030000 /* reg */
	},
	 {
		 NULL
	}
};