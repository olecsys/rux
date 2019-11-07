#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_user_data.h"
XUserData2::XUserData2()
{
	_udata0 = 0;
	_udata1 = 0;
};
XUserData2::XUserData2( void* udata0 , void* udata1 )
{
	_udata0 = udata0;
	_udata1 = udata1;
};
XUserData3::XUserData3()
{
	_udata0 = 0;
	_udata1 = 0;
	_udata2 = 0;
};
XUserData3::XUserData3( void* udata0 , void* udata1 , void* udata2 )
{
	_udata0 = udata0;
	_udata1 = udata1;
	_udata2 = udata2;
};