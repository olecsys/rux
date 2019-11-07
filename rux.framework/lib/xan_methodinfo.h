#ifndef XAN_METHODINFO_H
#define XAN_METHODINFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
#include "xan_methodparameter.h"
#include "xan_asm.h"
namespace rux
{	
	begin_declare_rux_class( MethodInfo );
		void* _address;
		::rux::XString _method_name;
	begin_declare_rux_class_members( MethodInfo );
	end_declare_rux_class_members( MethodInfo );
		XMethodInfo( char* name_ptr , void* address );
		XMethodInfo( const ::rux::XString& name , void* address );
		template< class T >
	#ifdef __x86__
		T RuntimeInvoke( rux_runtime_param_t* params_ptr , ::rux::uint32 params_count )
	#else
		T RuntimeInvoke( rux_runtime_param_t* params_ptr , ::rux::uint64 params_count )
	#endif
		{
			T result;
			if( (*this)()->_address )
				result = rux_runtime_invoke< T >( (*this)()->_address , params_ptr , params_count );
			return result;
		};
	end_declare_rux_class();
};
#endif
