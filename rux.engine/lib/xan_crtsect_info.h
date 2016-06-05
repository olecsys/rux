#ifndef XAN_CRTSECT_INFO_H
#define XAN_CRTSECT_INFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include <xan_crtsect.h>
class XCrtSectInfo
{
public:
	::rux::int32 _module_index;
	const char* _file;
	::rux::int32 _line;
	XCrtSect* _crt_sect;
	size_t _recursion;
	XCrtSectInfo( void );
};
#endif
