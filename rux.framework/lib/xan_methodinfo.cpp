#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_methodinfo.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
begin_implement_rux_class_ns( MethodInfo , rux );
	_address = NULL;
end_implement_rux_class();
namespace rux
{	
	XMethodInfo::XMethodInfo( char* name_ptr , void* address )
	{
		(*this)()->_method_name = name_ptr;
		(*this)()->_address = address;
	};
	XMethodInfo::XMethodInfo( const ::rux::XString& name , void* address )
	{
		(*this)()->_method_name = name;
		(*this)()->_address = address;
	};
};