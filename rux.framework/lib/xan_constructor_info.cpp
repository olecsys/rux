#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_constructor_info.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( ConstructorInfo , rux );
end_implement_rux_class();
namespace rux
{
	XGCRef& ConstructorInfo::Create( void )
	{
		return _ctor(); 
	};
};