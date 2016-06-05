#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_crtsect_info.h>
XCrtSectInfo::XCrtSectInfo( void )
{
	_file = 0;
	_line = 0;
	_crt_sect = 0;
	_module_index = 0;
	_recursion = 0;
};