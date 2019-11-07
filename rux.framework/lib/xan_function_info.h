#ifndef XAN_FUNCTION_INFO_H
#define XAN_FUNCTION_INFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_gchandle.h"
namespace rux
{		
	namespace type
	{		
		typedef void (*function_1_t)( const XGCRef* object , const XGCRef* param );
		typedef Object& (*return_function_1_t)( const XGCRef* object , const XGCRef* param );
	};
	begin_declare_rux_class( FunctionInfo );
		friend class ::Type;
		void Invoke( XObject& object , const XObject& param );
		void Invoke( const XGCRef* object , const XGCRef* param );
		XGCRef& InvokeResult( const XObject& object , const XObject& param );
		XGCRef& InvokeResult( const XGCRef& object , const XObject& param );
		XGCRef& InvokeResult( const XGCRef* object , const XGCRef* param );
		XGCRef& InvokeResult( const XObject& object , const XGCRef* param );
		XGCRef& InvokeResult( const XGCRef& object , const XGCRef* param );
	begin_declare_rux_class_members( FunctionInfo );
		::rux::uint32 _name_hash;	
		Type* _object_type;
		void* _function_address;
		XMallocArray< Type* > _param_types;
		::rux::uint8 _with_return;
	end_declare_rux_class_members( FunctionInfo );
		void Invoke( XObject& object , const XObject& param );
		void Invoke( const XGCRef* object , const XGCRef* param );
		XGCRef& InvokeResult( const XObject& object , const XObject& param );
		XGCRef& InvokeResult( const XGCRef& object , const XObject& param );
		XGCRef& InvokeResult( const XGCRef* object , const XGCRef* param );
		XGCRef& InvokeResult( const XObject& object , const XGCRef* param );
		XGCRef& InvokeResult( const XGCRef& object , const XGCRef* param );
	end_declare_rux_class();
};
#endif