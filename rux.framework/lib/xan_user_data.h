#ifndef XAN_USER_DATA_H
#define XAN_USER_DATA_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_diagnostic_defines.h>
#endif
struct XUserData2
{
	void* _udata0;
	void* _udata1;
	XUserData2( void );
	XUserData2( void* udata0 , void* udata1 );
};
struct XUserData3
{
	void* _udata0;
	void* _udata1;
	void* _udata2;
	XUserData3( void );
	XUserData3( void* udata0 , void* udata1 , void* udata2 );
};
