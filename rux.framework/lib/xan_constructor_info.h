#ifndef XAN_CONSTRUCTOR_INFO_H
#define XAN_CONSTRUCTOR_INFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
namespace rux
{	
	typedef XGCRef& (*ctor_t)( void );
	begin_declare_rux_class( ConstructorInfo );
		friend class ::Type;
		XGCRef& Create( void );
	begin_declare_rux_class_members( ConstructorInfo );
		Type* _object_type;
		ctor_t _ctor;
	end_declare_rux_class_members( ConstructorInfo );
		XGCRef& Create( void );
	end_declare_rux_class();
};
#endif