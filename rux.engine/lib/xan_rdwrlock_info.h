#ifndef XAN_RDWRLOCK_INFO_H
#define XAN_RDWRLOCK_INFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include <xan_crtsect.h>
class XRdWrLockInfo
{
public:
	::rux::int32 _module_index;
	const char* _file;
	::rux::int32 _line;
	::rux::threading::RdWrLock* _rdwrlock;
	size_t _recursion;
	XRdWrLockInfo( void );
};
#endif
